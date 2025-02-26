@page guidelines_howtoadr How to Create an ADR

## Introduction
Architecture Decision Records (ADRs) are documents that capture important architectural decisions made along with their context and consequences. ADR Tools is a command-line tool that helps in creating and managing ADRs.

## Prerequisites
- Ensure you have [ADR Tools](https://github.com/npryce/adr-tools) installed. 

### MacOS
    ```sh
    brew install adr-tools
    ```

### Linux
To install ADR Tools on a Linux system, you can use the following commands:

1. Clone the ADR Tools repository:
    ```sh
    git clone https://github.com/npryce/adr-tools.git
    ```

2. Navigate to the cloned directory:
    ```sh
    cd adr-tools
    ```

3. Add the `src` directory to your PATH:
    ```sh
    export PATH=$PATH:$(pwd)/src
    ```

4. Verify the installation:
    ```sh
    adr help
    ```

### Windows
To install ADR Tools on a Windows system, follow these steps:

1. Install [Git for Windows](https://gitforwindows.org/) if you don't have it already.

2. Open Git Bash and clone the ADR Tools repository:
    ```sh
    git clone https://github.com/npryce/adr-tools.git
    ```

3. Navigate to the cloned directory:
    ```sh
    cd adr-tools
    ```

4. Add the `src` directory to your PATH by editing your `.bashrc` or `.bash_profile` file:
    ```sh
    echo 'export PATH=$PATH:/c/path/to/adr-tools/src' >> ~/.bashrc
    source ~/.bashrc
    ```

5. Verify the installation:
    ```sh
    adr help
    ```

## Steps to Create an ADR

### Step 1: Create a New ADR
To create a new ADR, use the following command:
```sh
adr new <title-of-your-decision>
```
Replace `<title-of-your-decision>` with a short, descriptive title. For example:
```sh
adr new Use PostgreSQL as the Database
```
This will create a new ADR file in the `doc/architecture/decisions` directory with a unique identifier and the provided title.

### Step 3: Edit the ADR
Open the newly created ADR file in your preferred text editor. The file will have a template with sections such as:
- Title
- Status
- Context
- Decision
- Consequences

Fill in these sections with the relevant information about your architectural decision. Copilot can help you a lot! :) 

## Conclusion
Using ADR Tools simplifies the process of creating and managing Architecture Decision Records. By following the steps outlined in this tutorial, you can ensure that your architectural decisions are well-documented and easily accessible.

For more information, refer to the [ADR Tools documentation](https://github.com/npryce/adr-tools).
