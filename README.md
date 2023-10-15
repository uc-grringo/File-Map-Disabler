# File-Map-Disabler
-----------------------------
+ this is a simple tool that works for any process that CreateFileMapping(OpenFileMapping)
+ basically it just open handle to the communication via MapViewOfFile [LPVOID Handle = MapViewOfFile(MappingHandle, FILE_MAP_ALL_ACCESS, 0, 0, 0)]
+ then you can get all data simply [char* data = static_cast<char*>(Handle);]
+ now you can read all the sensitive data, that are being sent between modules
+ to clear all data you can do such [CopyMemory(data, emptyData, dataSize);]
+ target process might freeze for few seconds!
-----------------------------
  THIS PROJECT WAS MADE FOR TRAINING/LEARNING PURPOSE!
  IT WASN'T MADE TO DAMAGE ANY THIRD-PARTY SOFTWARE OR IT'S FUNCTIONALITY
