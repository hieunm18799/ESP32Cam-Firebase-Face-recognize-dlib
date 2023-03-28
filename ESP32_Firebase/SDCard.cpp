#include "SDCard.h"

int fileCountInFolder(String folder_name)
{
  int res;
  fs::FS &fs = SD; // sd card file system

  // discover the number of image files already stored in '/img' folder of the sd card and set image file counter accordingly
  res = -1;
  int tq = fs.mkdir(folder_name); // create the '/img' folder on sd card (in case it is not already there)
  if (tq)
  {
    // open the image folder and step through all files in it
    File root = fs.open(folder_name);
    while (true)
    {
      File entry = root.openNextFile(); // open next file in the folder
      if (!entry)
        break; // if no more files in the folder
      res++;   // increment image counter
      entry.close();
    }
    root.close();
  }

  return res;
}

bool createFile(String file_name_full_path, uint8_t *content, size_t length)
{
  fs::FS &fs = SD;

  Serial.printf("Storing image to sd card \n");

  File file = fs.open(file_name_full_path, FILE_WRITE); // create file on sd card
  if (!file)
    return false;
  else
  {
    if (file.write(content, length))
      return true;
    else
      return false;

    file.close(); // close image file on sd card
  }
}