# Time-Travelling File System

## 1. Introduction

This project implements a simplified, in-memory version control system inspired by Git. It manages versioned files with support for branching and historical inspection. The system is built from scratch in C++ and utilizes core data structures like Trees, HashMaps, and Heaps to manage file versions and system-wide metrics efficiently.

---

## 2. System Architecture

The file system is built upon three fundamental data structures, each implemented from the ground up:

- **Tree (TreeNode):**  
  The version history of each file is represented as a tree. A node in the tree (TreeNode) corresponds to a specific version of the file. Each node stores its content, a potential snapshot message, timestamps, a parent pointer, and a list of children, allowing for a branching version history.

- **HashMap (HashMap.hpp):**  
  A custom hash map is used to provide fast, O(1) average-time lookups. It is utilized in two key places:
  - Mapping filenames (string) to File objects within the FileSystem.
  - Mapping version IDs (integer) to TreeNode objects within each File, enabling quick access for operations like ROLLBACK.

- **Heap (Heap.hpp):**  
  A custom max-heap is used to efficiently track system-wide file metrics. The project uses two heaps:
  - A heap to track the most recently modified files based on their modification timestamp.
  - A heap to track files with the largest version trees (most edits) based on their total version count.

---

## 3. Key Semantics and Implementation Details

- **Immutability:**  
  Only snapshotted versions are immutable. Any attempt to INSERT into or UPDATE a snapshotted version will not modify it.

- **Automatic Versioning:**  
  When an INSERT or UPDATE command is executed on a snapshotted version, the system automatically creates a new version as a child of the current one. This new version becomes the active version. If the active version is not a snapshot, the modification happens in-place without creating a new version.

- **Version IDs:**  
  Version IDs are unique per file and are assigned sequentially, starting from 0 for the root.

---

## 4. How to Compile and Run

The project includes a shell script `run.sh` to simplify compilation and execution.

1. **Organize Files:**  
   Place all source files (`.cpp`) in a `src/` directory and all header files (`.hpp`) in an `include/` directory. The `run.sh` script should be in the parent directory of `src/` and `include/`.

2. **Make the script executable:**
   ```sh
   chmod +x run.sh
   ```

3. **Run the script:**
   ```sh
   ./run.sh
   ```
   The script will first clean any previous builds, then compile all `.cpp` files, and finally execute the program, presenting you with a `>>` prompt to enter commands.

---

## 5. Command Reference

The program accepts commands from standard input to interact with the file system.

### Core File Operations

#### `CREATE <filename>`
Creates a new file with the given name. An initial root version (ID 0) is created and snapshotted with the message "root".

**Output:**  
```
<filename> created...
```

**Example:**  
`CREATE mydoc`

---

#### `READ <filename>`
Displays the content of the file's currently active version.

**Output:**  
The content of the file, or an empty line if the content is empty.
```
File: <filename>, Version: <id>
Content: <file content>
```

**Example:**  
`READ mydoc`

---

#### `INSERT <filename> <content>`
Appends the given content to the file.

**Output (if new version is created):**
```
New version: <id> created...
File: <filename>, Version: <id>
Insertion done...
```
**Output (if modified in-place):**
```
File: <filename>, Version: <id>
Insertion done...
```
**Example:**  
`INSERT mydoc This is the first line.`

---

#### `UPDATE <filename> <content>`
Replaces the entire content of the file with the new content. Follows the same versioning logic as INSERT.

**Output (if new version is created):**
```
New version: <id> created...
File: <filename>, Version: <id>
Updated...
```
**Output (if modified in-place):**
```
File: <filename>, Version: <id>
<filename> updated...
```
**Example:**  
`UPDATE mydoc A completely new content.`

---

#### `SNAPSHOT <filename> <message>`
Marks the active version as a snapshot, making its content immutable. It stores the provided message and the current time. A version cannot be snapshotted more than once.

**Success Output:**  
```
File: <filename>, Version: <id>
Snapshot taken...
```

**Failure Output:** 
```
File: <filename>, Version: <id>
Snapshot already exists for this version...
```

**Example:**  
`SNAPSHOT mydoc First draft complete`

---

#### `ROLLBACK <filename> [versionID]`
Sets the active version to a different one.

- If `versionID` is provided, it switches the active version to the one with the specified ID.
- If no `versionID` is provided, it rolls back to the parent of the current active version.

**Output:**  
```
<filename> rolled back to version <id>...
```

**Example:**  
`ROLLBACK mydoc 1` or `ROLLBACK mydoc`

---

#### `HISTORY <filename>`
Lists all snapshotted versions on the direct path from the root to the currently active version.

**Output:**
```
<filename> history: 
Version: 0, Snapshot Message: root, Snapshot Timestamp: <time>
Version: 1, Snapshot Message: First draft complete, Snapshot Timestamp: <time>
```
**Example:**  
`HISTORY mydoc`

---

### System-Wide Analytics

#### `RECENT_FILES <num>`
Lists the `num` most recently modified files in descending order.

**Output:**  
A list of files and their last modification times.
```
<num> recent files:
File: <filename>, Modification Time: <time>
...
```

**Example:**  
`RECENT_FILES 3`

---

#### `BIGGEST_TREES <num>`
Lists the `num` files with the highest number of versions in descending order.

**Output:**  
A list of files and their total version counts.
```
<num> most edited files: 
File: <filename>, Versions: <total versions>
...
```

**Example:**  
`BIGGEST_TREES 5`

---

### Program Control

#### `EXIT`
Exits the program.

---

## 6. Error Handling

The system handles incorrect or inconsistent input gracefully with descriptive messages:

- **File already exists:**  
  `CREATE existing_file` -> `existing_file already exist...`

- **File not found:**  
  `READ missing_file` -> `missing_file not found...`

- **Missing arguments:**  
  `CREATE` -> `<filename> not provided`

- **Invalid arguments:**  
  `ROLLBACK mydoc abc` -> `Invalid versionID`

- **Filenames with spaces:**  
  `CREATE my file` -> `Filename can't have spaces`

- **Rollback from root:**  
  `ROLLBACK` (on root version) -> `No previous versions found...`

- **Heap query errors:**  
  `RECENT_FILES 5` (with only 3 files) -> `Requested 5 files but only 3 recent files are there...` or `No recent files were found...`

- **Unknown command:**  
  `DELETE mydoc` -> `Command not found...`

---

## 7. Sample Session

```
>> CREATE report
report created...
>> INSERT report Introduction to the project.
New version: 1 created...
File: report, Version: 1
Insertion done...
>> READ report
File: report, Version: 1
Content: Introduction to the project.
>> SNAPSHOT report Added introduction
File: report, Version: 1
Snapshot taken...
>> INSERT report This is the body paragraph.
New version: 2 created...
File: report, Version: 2
Insertion done...
>> READ report
File: report, Version: 1
Content: Introduction to the project. This is the body paragraph.
>> HISTORY report
report history: 
Version: 0, Snapshot Message: root, Snapshot Timestamp: Thu Sep 11 15:19:30 2025
Version: 1, Snapshot Message: Added introduction, Snapshot Timestamp: Thu Sep 11 15:19:45 2025
>> ROLLBACK report
report rolled back to version 1...
>> READ report
File: report, Version: 1
Content: Introduction to the project.
>> EXIT
```

