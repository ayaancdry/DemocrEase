# Project Title: DemocrEase (Voting System)

A sophisticated voting system designed to enhance the efficiency and fairness of large-scale elections.

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Credits](#credits)
- [Contact](#contact)

## Installation

To install the DemocrEase (Voting System), follow these steps:

1. Download and install the necessary libraries.

2. Compile all the source files.

3. Open the *b23cm1049_b23me1013_b23ph1020_b23cs1059_main.c* file.

4. In the terminal, execute:

    ```sh
    gcc -o b23cm1049_b23me1013_b23ph1020_b23cs1059_server_mainFxns b23cm1049_b23me1013_b23ph1020_b23cs1059_server_mainFxns.c -lws2_32
    ```

5. To start the server, execute:

    ```sh
    ./b23cm1049_b23me1013_b23ph1020_b23cs1059_server_mainFxns
    ```

6. In the terminal, execute:

    ```sh
    gcc -o b23cm1049_b23me1013_b23ph1020_b23cs1059_client_mainFxns b23cm1049_b23me1013_b23ph1020_b23cs1059_client_mainFxns.c -lws2_32
    ```

7. To run the client, execute:

    ```sh
    ./b23cm1049_b23me1013_b23ph1020_b23cs1059_client_mainFxns
    ```

8. To compile the bar graph generator, execute:

    ```sh
    gcc -o b23cm1049_b23me1013_b23ph1020_b23cs1059_client_barGraph b23cm1049_b23me1013_b23ph1020_b23cs1059_client_barGraph.c pbPlots.c supportLib.c -lm
    ```

9. To generate the bar graphs, execute:

    ```sh
    ./b23cm1049_b23me1013_b23ph1020_b23cs1059_client_barGraph
    ```

## Usage

The DemocrEase can be utilized for the following purposes:

1. Conducting statewide elections on a large scale.
2. Managing elections across multiple constituencies simultaneously.
3. Generating user IDs and passwords for electoral registration.
4. Ensuring secure elections by encrypting data transmission between the server and clients.

To conduct an election:

- Ensure all necessary setups and configurations are complete.
- Run the executable files.
- Follow the on-screen prompts to initiate and manage the election process.

## Configuration

The voting system allows configuration of various parameters, including:

1. Number of voting options.
2. Candidate names and IDs.
3. User authentication settings.
4. These settings can be adjusted by modifying the configuration files or database tables.

## Credits

This project utilizes the following resources:

- [pbPlots](https://github.com/InductiveComputerScience/pbPlots/blob/master/C/pbPlots.h)

## Contact

For inquiries or support, please contact:

1. Yash Singh (B23CM1049) - b23cm1049@iitj.ac.in
2. Ayaan Choudhury (B23ME1013) - b23me1013@iitj.ac.in
3. Sarthak Sharma (B23PH1020) - b23ph1020@iitj.ac.in
4. Saher Dev (B23CS1059) - b23cs1059@iitj.ac.in
