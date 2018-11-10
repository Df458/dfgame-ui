#ifndef DF_UI_FRAME_IO
#define DF_UI_FRAME_IO

#include "frame_data.h"
#include "resource/xmlutil.h"

// Loads a frame from path
void load_frame(const char* path, frame_data* f);

// Saves a frame to path
void save_frame(const char* path, const frame_data* f);

// Read a frame's data from xml. The frame can contain properties or a file reference.
void xml_read_frame(xmlNodePtr node, frame_data* f, const char* path);

// Write a frame's data to xml
// If force_write_properties is false and resource_path is non-NULL,
// a reference to the frame's file will be written.
void xml_write_frame(xmlTextWriterPtr writer, const frame_data* f, bool force_write_properties);

#endif // DF_UI_FRAME_IO
