# File Size Analyzer

## Project Overview

The File Size Analyzer is a multi-threaded desktop application built with Qt. It's designed to efficiently scan a specified root directory, list all files and directories, and calculate their cumulative size on disk. The results are displayed in a clean, navigable tree view, allowing users to quickly identify large files and folders that may be consuming disk space.

The application leverages Qt's multi-threading capabilities to perform file system analysis in the background, ensuring the user interface remains responsive and fluid.

## Features

* **Multi-threaded Analysis**: The application uses a `QThreadPool` to distribute the disk-scanning workload across multiple threads, preventing the UI from freezing.
* **Hierarchical View**: The analysis results are displayed in a `QTreeView`, providing an intuitive, expandable tree structure for easy navigation.
* **User-Friendly Display**: File sizes are presented in a human-readable format (e.g., KB, MB, GB).
* **Sortable Columns**: Users can sort the file and directory list by name or size in ascending or descending order.
* **Robust Exit**: The application handles a clean shutdown by ensuring all background threads have completed their tasks before exiting.

---

## Build and Run

### Prerequisites

* **Qt 6** (or later)
* **C++ Compiler** (MSVC, GCC, or Clang)
* **Git**

### Building from Source

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
    cd your-repo-name
    ```

2.  **Open the project in Qt Creator:**
    * Launch **Qt Creator**.
    * Go to `File > Open File or Project` and select the `.pro` or `CMakeLists.txt` file in the cloned repository.

3.  **Build the project:**
    * Configure the project for your desired kit (e.g., Desktop Qt 6.x.x).
    * Click `Build > Build Project "your-project-name"`.

### Running the Application

After a successful build, you can run the application directly from **Qt Creator** or find the executable in your build directory.

1.  **From Qt Creator:**
    * Click `Run` (the green play button) in the bottom-left corner.

2.  **From the command line:**
    * Navigate to your build directory.
    * Execute the application:
        ```bash
        ./your-executable-name
        ```

---

## Contributing

We welcome contributions! If you find a bug or have an idea for a new feature, please open an issue or submit a pull request.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

## Credits

* **Frederik Vannoote** - Initial work and development.
