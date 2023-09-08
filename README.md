
# System Monitor

System Monitor is a versatile command-line tool written in C that provides real-time information about your system's performance and resource utilization. This program allows you to customize the output and choose what information you want to see.

## Features

-   Display real-time information about:
    
    -   CPU usage
        
    -   Memory usage
        
    -   Disk usage
                
    -   System uptime
        
    -   User information
        
-   Customize the output with various options:
    
    -   Choose between system or user-specific information
        
    -   Display output sequentially
        
    -   Specify the number of samples and time delay between samples
        
    -   Visualize data with optional graphics output
        

## Usage

To use System Monitor, follow these basic usage guidelines:
Copy Code
`./main [OPTIONS]`

Replace `[OPTIONS]` with one or more of the following command-line arguments:

-   `--system`: Display system-specific information (default).
    
-   `--user`: Display user-specific information.
    
-   `--graphics`: Enable graphical output.
    
-   `--sequential`: Display output sequentially.
    
-   `--samples=NUM`: Set the number of samples (default: 10).
    
-   `--tdelay=SECONDS`: Set the time delay between samples in seconds (default: 1).
    

### Examples

1.  Display system-specific information in a sequential manner with graphics:
    
    Copy code
    `./main --system --sequential --graphics`
    
2.  Monitor user-specific information with a sample size of 5 and a delay of 2 seconds between samples:
    
    Copy code
    `./main --user --samples=5 --tdelay=2`
    
3.  Display both system and user information in the default mode:
    
    Copy code
    `./main`
    

## Build and Installation

To build and run System Monitor, follow these steps:

1.  Clone the repository:
    
    Copy code
    `git clone https://github.com/yourusername/system-monitor.git`
    
2.  Navigate to the project directory:
    
    Copy code
    `cd system-monitor`
    
3.  Build the program:
    
    Copy code
    `gcc -o main main.c -L. lib_syslib.a`
    
4.  Run the program:
    
    bashCopy code
    `./main [OPTIONS]`
    

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and create a pull request with your changes.


## Contact

Email - kaushik445566@gmail.com
Github Profile - https://github.com/ikaushiksharma
