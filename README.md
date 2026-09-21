# Semester-2-PROJECT

A console-based university management system written in C++.

## Features

- Add and list students and professors
- Calculate student GPA from five assessment scores
- Track fee status and current semester
- Store and print eight-semester transcripts
- View the BS Computer Science semester plan
- Persist records in `SEMESTER-2-PROJECT/university_data.txt`
- Recover gracefully from invalid menu input and malformed saved records

## Build and run

From the repository root:

```text
g++ -std=c++11 -Wall -Wextra SEMESTER-2-PROJECT/project.cpp -o university
./university
```

On Windows, run the generated executable as `university.exe`.