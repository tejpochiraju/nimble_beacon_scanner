| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- |

# Notes

- Derived from the [Nimble BLE Central example](https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/nimble/blecent)
    - Removed all code except related to scan + discovery
- Changed to active scan and increaed scan interval
    - Active scan is needed to request and return scan response from beacons which includes information such as temperature, humidity, battery, acceleration

# Example output

## Active Scan

```bash
I (990) BLE_SCAN: Target Device Found: BC:57:29:05:90:11
I (990) BLE_SCAN: Raw Advertisement Data:
I (990) BLE_SCAN: 02 
I (990) BLE_SCAN: 01 
I (1000) BLE_SCAN: 06 
I (1000) BLE_SCAN: 03 
I (1000) BLE_SCAN: 03 
I (1000) BLE_SCAN: AA 
I (1010) BLE_SCAN: FE 
I (1010) BLE_SCAN: 15 
I (1010) BLE_SCAN: 16 
I (1020) BLE_SCAN: AA 
I (1020) BLE_SCAN: FE 
I (1020) BLE_SCAN: 21 
I (1030) BLE_SCAN: 04 
I (1030) BLE_SCAN: 0F 
I (1030) BLE_SCAN: 0B 
I (1030) BLE_SCAN: D4 
I (1040) BLE_SCAN: 18 
I (1040) BLE_SCAN: AB 
I (1040) BLE_SCAN: 43 
I (1050) BLE_SCAN: 89 
I (1050) BLE_SCAN: 02 
I (1050) BLE_SCAN: 08 
I (1060) BLE_SCAN: FF 
I (1060) BLE_SCAN: 48 
I (1060) BLE_SCAN: FD 
I (1060) BLE_SCAN: 11 
I (1070) BLE_SCAN: 01 
I (1070) BLE_SCAN: 18 
I (1070) BLE_SCAN: 92 
I (1080) BLE_SCAN: Temperature: 24.67 °C // Correct
```

## Passive Scan

```bash
I (850) BLE_SCAN: Scanning started...
I (1030) BLE_SCAN: Target Device Found: BC:57:29:05:90:11
I (1030) BLE_SCAN: Raw Advertisement Data:
I (1030) BLE_SCAN: 02 
I (1030) BLE_SCAN: 01 
I (1030) BLE_SCAN: 06 
I (1040) BLE_SCAN: 03 
I (1040) BLE_SCAN: 03 
I (1040) BLE_SCAN: AA 
I (1040) BLE_SCAN: FE 
I (1050) BLE_SCAN: 11 
I (1050) BLE_SCAN: 16 
I (1050) BLE_SCAN: AA 
I (1060) BLE_SCAN: FE 
I (1060) BLE_SCAN: 20 
I (1060) BLE_SCAN: 00 
I (1070) BLE_SCAN: 0B 
I (1070) BLE_SCAN: D4 
I (1070) BLE_SCAN: 18 
I (1080) BLE_SCAN: B0 
I (1080) BLE_SCAN: 00 
I (1080) BLE_SCAN: 20 
I (1080) BLE_SCAN: B1 
I (1090) BLE_SCAN: D0 
I (1090) BLE_SCAN: 01 
I (1090) BLE_SCAN: 20 
I (1100) BLE_SCAN: 04 
I (1100) BLE_SCAN: 78 
I (1100) BLE_SCAN: Temperature: 176.00 °C // Incorrect
I (1110) BLE_SCAN: -----------------------------------
```