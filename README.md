# V2C_Based_ADAS_Project_ITI_GP

![Cover](https://github.com/mhmdreda99/V2C_Based_ADAS_Project_ITI_GP/blob/master/Docs/images/nexusjpeg)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) 

## Overview
The V2C ADAS Nexus project aims to create a cutting-edge embedded system that seamlessly integrates safety and connectivity features for the automotive industry. By combining Advanced Driver Assistance Systems (ADAS) with Vehicle-to-Cloud (V2C) communication, this project intends to enhance vehicle safety, improve driver experience, and redefine the standard for automotive technology.


## Table of Contents
- [V2C\_Based\_ADAS\_Project\_ITI\_GP](#v2c_based_adas_project_iti_gp)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Configuration](#configuration)
    - [Installation](#installation)
    - [To-Do](#to-do)
    - [Project Structure](#project-structure)
    - [STM32 Board](#stm32-board)
    - [Raspberry Pi Components](#raspberry-pi-components)
    - [Dashboard Access](#dashboard-access)
  - [Branches](#branches)
  - [Authors](#authors)
  - [License](#license)
  - [Acknowledgments](#acknowledgments)


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

Ensure you have the following installed:

- [Raspberry Pi](https://www.raspberrypi.org/) with Raspbian OS
- [STM32 IDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- [Python](https://www.python.org/) (latest version)
-  [PHP] (Version 7.4)
-  [XMPP_Server] (e.g., ejabberd, Prosody)
-  [AWS SDK for PHP](https://github.com/aws/aws-sdk-php) (Version 3)
-  AWS account with S3 bucket configured


### Configuration
```bash
; Example configuration in a .env file
AWS_ACCESS_KEY_ID=your_access_key
AWS_SECRET_ACCESS_KEY=your_secret_key
AWS_DEFAULT_REGION=your_region
AWS_BUCKET_NAME=your_bucket_name
```

### Installation

1. Clone the repository to your Raspberry Pi:

    ```bash
    git clone https://github.com/mhmdreda99/V2C_Based_ADAS_Project_ITI_GP.git
    ```

2. Open the STM32 IDE and load the STM32 project from the `STM32` directory.

3. Install Python dependencies:

    ```bash
    pip install -r requirements.txt
    ```

4. Follow the specific setup steps for your project components.

### To-Do

Before running the project, make sure to complete the following to-do items:

- Configure the Raspberry Pi with necessary dependencies.
- Connect and configure the STM32 board.
- Any additional project-specific steps.

### Project Structure
```bash
- .vscode                  # Visual Studio Code settings
- AWS_v1 
  - datatransfer.py              -- Python script for data transfer
  - datatransfer_PC.py           -- Python script for data transfer on a PC                  # AWS-related code or configuration (e.g., Lambda functions)
  - dataReceive_PC.py              -- Python script for data receiving on a PC
 - dataReceive.py                 -- Modified version of dataReceive.py

- BlindSpotAssistant       # Main folder for the Blind Spot Assistant feature
  - v1.0                   # Version 1.0 of the Blind Spot Assistant
  - v2.0                   # Version 2.0 of the Blind Spot Assistant
  - v3.0                   # Version 3.0 of the Blind Spot Assistant
  - v4.0                   # Version 4.0 of the Blind Spot Assistant
  - v5.0                   # Version 5.0 of the Blind Spot Assistant (Accepted)
    - v3.0                 # Subversion for version 3.0 (Accepted )
- GPS                      # GPS-related code or configuration
- HostingGUI               # Code related to hosting a graphical user interface (NEXUS Website https://nexus.mhmdreda99.com/)
 - css
    - style.css               # CSS stylesheets
  - img
    - BG.svg
    - ITINexus.svg
    - ITINexusWhite.svg
    - User.svg
    - clock-8701712-7097715.webp
    - cloudy-weather-3311758-2754892.png
    - fuel-can-5019532-4185548.png
    - lock.svg
    - motion-sensor-4035927-3342606.webp
    - route-destination-5480363-4569725.png
    - scroll-map-8616025-6815679.png
  - js
    - maps.js                 # JavaScript files
    - mapsBackup.js
    - script.js
  - vendor                    # Composer dependencies
    - aws
    - bin
    - composer
    - guzzlehttp
    - mtdowling
      - jmespath.php
    - psr
    - ralouphie
      - getallheaders
    - symfony
  - autoload.php              # Composer autoload file
  - DataFetch.php             # PHP script for data fetching
- ITI_Nexus_Code_11_11_2023_4AM.zip   # Archive file containing code (dated 11.11.2023 4 AM)
- ITI_Nexus_Code_12_11_2023_2AM.zip   # Archive file containing code (dated 12.11.2023 2 AM)
- composer.json              # Composer dependencies file
- composer.lock              # Composer lock file
- index.html                 # HTML file for the main page
- index.php                  # PHP file for server-side logic
- login.php                  # PHP file for handling login functionality
- MQTT                     # MQTT-related code or configuration
  - v2.0                   # Version 2.0 of the MQTT code
  - with_GUI               # MQTT code with a graphical user interface
    - v2.0                 # Version 2.0 of MQTT with GUI
- gateway                  # Gateway-related code or configuration
  - gateWay_Assis_UART_WAS.py #  reads data from a UART (Universal Asynchronous Receiver-Transmitter) connection, processes the data, reads GPS coordinates from files, and publishes the processed data to AWS IoT (Internet of Things) using MQTT (Message Queuing Telemetry Transport). The script also listens for commands from a web service 
- spi_task                 # SPI-related code or configuration for communtcation between componenets (rejected )
- LICENSE                  # Project license file
- README.md                # Project README file
- Test_suit.xlsx           # Test suite or unit & integration test-related documents

```


{## Running the Project

### STM32 Board

1. Follow the instructions in your STM32 IDE documentation to build and upload the firmware to the STM32 board.

### Raspberry Pi Components

1. Execute the necessary Python scripts or commands on the Raspberry Pi. These scripts might be related to data transfer or any other functionality used in your project.

### Dashboard Access

Visit the [Dashboard](https://nexus.mhmdreda99.com/).

- **Login Credentials:**
  - Username: ITI username
  - Password: 123 password

Make sure to meet all prerequisites and ensure that the STM32 board and Raspberry Pi components are correctly connected and configured before running the project.}

## Branches

- `master` - Default branch
- `AWS`
- `BlindSpotAssistant`
- `Feature_Voice_Recognition`
- `GPS`
- `MQTT`
- `OBD_Timer`
- `OBD_Timer2`
- `OBD_WaterLevel`
- `OBD`
- `gateWayUART`
- `mqtt`


## Authors

* *Mohamed Reda* - *Team Leader* - [mhmdreda99](https://github.com/mhmdreda99)

- Amr Darwish [Amr Darwish](https://github.com/AmrDarwish)
- Abdel-Hamid Tarek - [Abdel-Hamidd](https://github.com/Abdel-Hamidd)
- Yasmen Ehab - [YasmenEhab](https://github.com/YasmenEhab)
-  Mostafa Ibrahim - [MostafaIibrahim](https://github.com/MostafaIibrahim)
-  Mario Vector [MarioVictor](https://github.com/Mariovictore)

See also the list of [contributors](https://github.com/mhmdreda99/V2C_Based_ADAS_Project_ITI_GP/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

- Thanks to our great Supervisors  **Joe Nofal** , **Nour**- *Training Heads* 
