#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"

#define BLE_ADDR_STR_LEN 18
#define TARGET_BLE_ADDR "BC:57:29:05:90:11"

static const char *tag = "BLE_SCAN";

/* Helper to format BLE address as a string */
static void format_ble_addr(const ble_addr_t *addr, char *addr_str) {
    snprintf(addr_str, BLE_ADDR_STR_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
             addr->val[5], addr->val[4], addr->val[3], addr->val[2],
             addr->val[1], addr->val[0]);
}

/* GAP event callback to process discovered devices */
static int blecent_gap_event(struct ble_gap_event *event, void *arg) {
    if (event->type == BLE_GAP_EVENT_DISC) {
        char addr_str[BLE_ADDR_STR_LEN];
        const uint8_t *raw_data = event->disc.data;
        uint8_t raw_data_len = event->disc.length_data;

        /* Format the device address */
        format_ble_addr(&event->disc.addr, addr_str);

        /* Check if this is the target device */
        if (strcmp(addr_str, TARGET_BLE_ADDR) != 0) {
            return 0; // Ignore other devices
        }

        ESP_LOGI(tag, "Target Device Found: %s", addr_str);

        /* Log raw advertisement data */
        ESP_LOGI(tag, "Raw Advertisement Data:");
        for (int i = 0; i < raw_data_len; i++) {
            ESP_LOGI(tag, "%02X ", raw_data[i]);
        }

        /* Parse and process custom data */
        if (raw_data_len > 17) { // Ensure sufficient data length
            uint8_t high_byte = raw_data[16];
            uint8_t low_byte = raw_data[17];
            float temperature = (high_byte << 8 | low_byte) / 256.0f;

            ESP_LOGI(tag, "Temperature: %.2f °C", temperature);
        } else {
            ESP_LOGI(tag, "Custom Data: [Insufficient length]");
        }

        ESP_LOGI(tag, "-----------------------------------");
    }

    return 0;
}

/* Start scanning for BLE devices */
static void blecent_scan(void) {
    uint8_t own_addr_type;
    struct ble_gap_disc_params disc_params = {0};

    /* Determine the address type to use */
    int rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0) {
        ESP_LOGE(tag, "Error determining address type; rc=%d", rc);
        return;
    }

    /* Set scan parameters */
    disc_params.filter_duplicates = 1;
    disc_params.passive = 0;  // Enable active scanning
    disc_params.limited = 1;  // Has no impact on result
    disc_params.itvl = 160;   // Scan interval: 100ms (160 * 0.625ms)
    disc_params.window = 158; // Scan window: 99ms (158 * 0.625ms)

    rc = ble_gap_disc(own_addr_type, BLE_HS_FOREVER, &disc_params, blecent_gap_event, NULL);
    if (rc != 0) {
        ESP_LOGE(tag, "Error starting scan; rc=%d", rc);
    } else {
        ESP_LOGI(tag, "Scanning started...");
    }
}

/* Callback when the BLE host is reset */
static void blecent_on_reset(int reason) {
    ESP_LOGE(tag, "Resetting state; reason=%d", reason);
}

/* Callback when the BLE host is synchronized */
static void blecent_on_sync(void) {
    ESP_LOGI(tag, "BLE host synchronized. Starting scan...");
    blecent_scan();
}

/* BLE host task */
void blecent_host_task(void *param) {
    ESP_LOGI(tag, "BLE Host Task Started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

void app_main(void) {
    esp_err_t ret;

    /* Initialize NVS */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Initialize NimBLE */
    ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(tag, "Failed to init NimBLE; rc=%d", ret);
        return;
    }

    /* Set BLE host configuration */
    ble_hs_cfg.reset_cb = blecent_on_reset;
    ble_hs_cfg.sync_cb = blecent_on_sync;

    /* Start BLE host task */
    nimble_port_freertos_init(blecent_host_task);
}
