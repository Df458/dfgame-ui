// Log category, used to filter logs
#define LOG_CATEGORY "UI"

#include "frame_io.h"

#include "core/check.h"
#include "core/stringutil.h"
#include "resource/paths.h"
#include "resource/texture_loader.h"

// Loads a frame from path
void load_frame(const char* path, frame_data* f) {
    check_return(f != NULL, "Frame is NULL", );

    xmlDocPtr doc = xmlReadFile(path, NULL, 0);
    check_return(doc, "Failed to load frame at path %s", , path);

    xmlNodePtr root = xml_match_name(xmlDocGetRootElement(doc), "frame");
    check_return(root != NULL, "Frame file %s is invalid", , path);

    char* temp_path = NULL;
    if(xml_property_read(root, "path", &temp_path)) {
        error("Frame file %s redirects to %s, this is not allowed", path, temp_path);
        sfree(temp_path);
        return;
    }

    xml_read_frame(root, f, path);
    f->asset_path = nstrdup(path);

    xmlFreeDoc(doc);
}

// Saves a frame to path
void save_frame(const char* path, const frame_data* f) {
    xmlTextWriter* writer = xmlNewTextWriterFilename(path, 0);
    check_return(writer, "Failed to open path %s for writing", , path);

    xmlTextWriterStartDocument(writer, NULL, "ISO-8859-1", NULL);

    xml_write_frame(writer, f, true);

    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);
}

// Read a frame's data from xml. The frame can contain properties or a file reference.
void xml_read_frame(xmlNodePtr node, frame_data* f, const char* path) {
    uint16 margin = 0;
    char* texture_path = NULL;
    aabb_2d box = aabb_2d_zero;

    xml_property_read(node, "margin", &margin);
    xml_property_read(node, "position", &box.position);
    xml_property_read(node, "dimensions", &box.dimensions);
    if(!xml_property_read(node, "texture", &texture_path)) {
        error("Can't load a frame without a texture");
        return;
    }

    texture_path = combine_paths(get_folder(path), texture_path, true);
    gltex tex = load_texture_gl(texture_path);
    sfree(texture_path);

    frame_data_new_default(f, tex, box, margin);
}

// Write a frame's data to xml
// If force_write_properties is false and resource_path is non-NULL,
// a reference to the frame's file will be written.
void xml_write_frame(xmlTextWriterPtr writer, const frame_data* f, bool force_write_properties) {
    xmlTextWriterStartElement(writer, (xmlChar*)"frame");

    if(!force_write_properties && f->asset_path != NULL) {
        xml_property_write(writer, "path", f->asset_path);
    } else {
        uint16 margin = 0;
        aabb_2d box = aabb_2d_zero;

        // TODO: Add support for varying boxes per-side
        box.position = f->uvs[0].position;
        box.dimensions = vec2_sub(vec2_add(f->uvs[8].position, f->uvs[8].dimensions), box.position);
        margin = (uint16)f->uvs[0].dimensions.x;

        xml_property_write(writer, "margin", margin);
        xml_property_write(writer, "position", box.position);
        xml_property_write(writer, "dimensions", box.dimensions);
        xml_property_write(writer, "texture", f->texture.asset_path);
    }

    xmlTextWriterEndElement(writer);
}
