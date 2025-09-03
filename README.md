# JPEG Metadata Remover

A simple, command-line tool written in C for removing EXIF metadata from JPEG image files to protect your privacy.

## Features

* Removes the entire EXIF (APP1) data block from JPEG files.
* Creates a new, clean image file, leaving the original untouched.
* Cross-platform and compiles with standard tools like GCC or Clang.

## Building from Source

### Prerequisites

* A C compiler (e.g., GCC or Clang)
* The `make` utility

### Instructions

Clone the repository and run `make`:

```bash
git clone [https://github.com/Tech-Reformist/metadata-cleaner.git](https://github.com/Tech-Reformist/metadata-cleaner.git)
cd your-repo-name
make
```

This will create an executable file named `remover` in the project directory.

## Usage

Run the program from your terminal, providing the source image and the desired destination file name.

```bash
./remover source_image.jpg clean_image.jpg
```
A new file named `clean_image.jpg` will be created without the EXIF metadata.

## License

This project is licensed under the AGPLv3 License. See the `LICENSE` file for details.