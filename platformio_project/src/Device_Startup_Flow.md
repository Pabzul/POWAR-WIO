# Device Startup Flow

This document describes the flow of the device startup process.

- [x] device turns on
- [ ] wifi connection flow
  - [x] handle creating new wifi connection
  - [x] handle connecting to existing wifi connection
  - [ ] handle resetting wifi connection settings
- [ ] ownership to device pairing flow
  - [ ] read pairing info from SD Card
    - [ ] if no info present
      - [ ] ask to create new pairing request
        - [ ] request 4 digit pairing code from web platform via HTTP request
          - [ ] if fails - show error - ask to try again
        - [ ] display pairing code to user
          - [ ] user will enter this code on the web platform
        - [ ] ask for user to press button on device to make confirmation HTTP request
          - [ ] if confirmation HTTP request fails
            - [ ] error message - ask to try again
          - [ ] if confirmation request returns unconfirmed status
            - [ ] error message - ask to try again
          - [ ] if confirmation request returns confirmed status
            - [ ] success message
            - [ ] save pairing info to SD Card
            - [ ] ask to connect to MQTT broker
    - [ ] if info present
      - [ ] attempt to connect to MQTT broker
        - [ ] if connection fails
          - [ ] ask to try again or to create a new pairing request
          - [ ] if trying again
            - [ ] attempt to connect to MQTT broker
          - [ ] if creating new pairing request
            - [ ] ask to create new pairing request
        - [ ] if connection succeeds
          - [ ] subscribe to device CITY topics
          - [ ] publish sensor info to device POWAR topics
        - [ ]
