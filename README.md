# English Summary: Student Management Program

This program manages student information using binary files, single linked lists, and the QISAM direct access method.

## Main Features

### Menu Interface
- Provides multiple options for student data management

### Data Input Methods
- **Keyboard Input in Table Format**
  - Successfully adds student information
  - Checks for duplicate student IDs
  - Stops input when an empty ID is entered
  - Allows returning to input mode from the menu
  - Data is automatically saved to file

- **Binary File Input**
  - Can load existing student data from binary files
  - When adding more data, using the same filename appends to the existing file
  - Using a different filename creates a new file

### Student Information Management
- **View Student List**
  - Displays all entered student information

- **Search Functions**
  - Search by student ID or name
  - Press Enter to search after typing keywords
  - Navigate between search results using left/right arrow keys
  - Exit search by leaving both ID and Name fields empty

- **Filter Students**
  - Can filter out students with failing grades (below 4)
  - Filtered students are exported to a separate text file and removed from the binary file

- **Update Student Information**
  - Enter student ID to update their information
  - Press Enter to skip fields you don't want to update
  - Cancel update by pressing Enter when prompted for student ID
  - External file data is automatically updated

- **Sort Student List**
  - Sort by name or grade using "asc" or "desc" commands
  - Continue sorting with left/right arrow keys
  - Sorting is canceled when neither column contains "asc" or "desc"

- **Delete Students**
  - Remove students by ID
  - Cancel deletion by pressing Enter without entering an ID

### Data Export Options
- **Export to Text File**
  - Creates a human-readable text file of student information
  
- **Export to Binary File**
  - Saves data for future use
