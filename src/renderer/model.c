#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/io.h"
#include "../data_structures/dynamic_array.h"
#include "vertex.h"

char *skip_whitespace(char *pointer) {
    while (*pointer == ' ' || *pointer == '\t') {
        pointer++;
    }

    return pointer;
}

char *skip_non_whitespace(char *pointer) {
    while (*pointer != ' ' && *pointer != '\t') {
        pointer++;
    }

    return pointer;
}

char *skip_line(char *pointer) {
    while (*pointer != '\n') {
        pointer++;
    }

    return ++pointer;
}

struct vertex_coordinate parse_vertex_coordinate(char *line) {
    char *pointer = line + 2;

    struct vertex_coordinate output;

    output.x = strtof(pointer, &pointer);
    output.y = strtof(pointer, &pointer);
    output.z = strtof(pointer, &pointer);

    return output;
}

struct vertex_texture_coordinate parse_vertex_texture_coordinate(char *line) {
    char *pointer = line + 3;

    struct vertex_texture_coordinate output;

    output.x = strtof(pointer, &pointer);
    output.y = strtof(pointer, &pointer);

    return output;
}

struct vertex_normal parse_vertex_normal(char *line) {
    char *pointer = line + 3;

    struct vertex_normal output;

    output.x = strtof(pointer, &pointer);
    output.y = strtof(pointer, &pointer);
    output.z = strtof(pointer, &pointer);

    return output;
}

struct face_element parse_face_element(char *string,
                                       unsigned int coordinates_count,
                                       unsigned int texture_coordinates_count,
                                       unsigned int normals_count) {
    struct face_element element = {0, 0, 0};

    int parsed_index = atoi(strsep(&string, "/"));

    if (parsed_index > 0) {
        element.coordinate_index = parsed_index;
    } else if (parsed_index < 0) {
        element.coordinate_index = parsed_index + coordinates_count + 1;
    }

    parsed_index = atoi(strsep(&string, "/"));

    if (parsed_index > 0) {
        element.texture_coordinate_index = parsed_index;
    } else if (parsed_index < 0) {
        element.texture_coordinate_index =
            parsed_index + texture_coordinates_count + 1;
    }

    parsed_index = atoi(strsep(&string, "/"));

    if (parsed_index > 0) {
        element.normal_index = parsed_index;
    } else if (parsed_index < 0) {
        element.normal_index = parsed_index + normals_count + 1;
    }

    return element;
}

struct face parse_face(char *line, unsigned int coordinates_count,
                       unsigned int texture_coordinates_count,
                       unsigned int normals_count) {
    char *pointer = line + 2;

    struct dynamic_array face_elements;
    dynamic_array_init(&face_elements, sizeof(struct face_element));

    char *element = strtok(pointer, " ");

    while (element != NULL) {
        struct face_element new_element =
            parse_face_element(element, coordinates_count,
                               texture_coordinates_count, normals_count);
        dynamic_array_insert_end(&face_elements, &new_element);

        element = strtok(NULL, " ");
    }

    struct face face;

    face.face_elements = face_elements.array;
    face.face_element_count = face_elements.element_count;

    return face;
}

struct vertex construct_vertex(struct dynamic_array *coordinates,
                               struct dynamic_array *texture_coordinates,
                               struct dynamic_array *normals,
                               unsigned int coordinate_index,
                               unsigned int texture_coordinate_index,
                               unsigned int normal_index) {
    struct vertex vertex;

    struct vertex_coordinate *coordinate =
        dynamic_array_get(coordinates, coordinate_index - 1);

    vertex.x = coordinate->x;
    vertex.y = coordinate->y;
    vertex.z = coordinate->z;

    if (texture_coordinate_index > 0) {
        struct vertex_texture_coordinate *texture_coordinate =
            dynamic_array_get(texture_coordinates,
                              texture_coordinate_index - 1);

        vertex.texture_x = texture_coordinate->x;
        vertex.texture_y = texture_coordinate->y;
    } else {
        vertex.texture_x = 0;
        vertex.texture_y = 0;
    }

    if (normal_index > 0) {
        struct vertex_normal *normal =
            dynamic_array_get(normals, normal_index - 1);

        vertex.normal_x = normal->x;
        vertex.normal_y = normal->y;
        vertex.normal_z = normal->z;
    } else {
        vertex.normal_x = 0;
        vertex.normal_y = 0;
        vertex.normal_z = 0;
    }

    vertex.r = (vertex.normal_x + 1) / 2;
    vertex.g = (vertex.normal_y + 1) / 2;
    vertex.b = (vertex.normal_z + 1) / 2;

    return vertex;
}

struct model model_from_obj(const char *path) {
    struct dynamic_array coordinates;
    struct dynamic_array texture_coordinates;
    struct dynamic_array normals;
    struct dynamic_array faces;

    dynamic_array_init(&coordinates, sizeof(struct vertex_coordinate));
    dynamic_array_init(&texture_coordinates,
                       sizeof(struct vertex_texture_coordinate));
    dynamic_array_init(&normals, sizeof(struct vertex_normal));
    dynamic_array_init(&faces, sizeof(struct face));

    FILE *file = fopen(path, "r");

    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            struct vertex_coordinate coordinate = parse_vertex_coordinate(line);
            dynamic_array_insert_end(&coordinates, &coordinate);
        }

        if (line[0] == 'v' && line[1] == 't') {
            struct vertex_texture_coordinate texture_coordinate =
                parse_vertex_texture_coordinate(line);
            dynamic_array_insert_end(&texture_coordinates, &texture_coordinate);
        }

        if (line[0] == 'v' && line[1] == 'n') {
            struct vertex_normal normal = parse_vertex_normal(line);
            dynamic_array_insert_end(&normals, &normal);
        }

        if (line[0] == 'f') {
            struct face face = parse_face(line, coordinates.element_count,
                                          texture_coordinates.element_count,
                                          normals.element_count);
            dynamic_array_insert_end(&faces, &face);
        }
    }

    fclose(file);

    struct dynamic_array vertices;
    dynamic_array_init(&vertices, sizeof(struct vertex));

    unsigned int triangle_count = 0;

    for (int f = 0; f < faces.element_count; f++) {
        struct face *face = dynamic_array_get(&faces, f);

        for (int i = 0; i <= face->face_element_count - 3; i++) {
            struct vertex vertex1 = construct_vertex(
                &coordinates, &texture_coordinates, &normals,
                face->face_elements[0].coordinate_index,
                face->face_elements[0].texture_coordinate_index,
                face->face_elements[0].normal_index);

            struct vertex vertex2 = construct_vertex(
                &coordinates, &texture_coordinates, &normals,
                face->face_elements[i + 1].coordinate_index,
                face->face_elements[i + 1].texture_coordinate_index,
                face->face_elements[i + 1].normal_index);

            struct vertex vertex3 = construct_vertex(
                &coordinates, &texture_coordinates, &normals,
                face->face_elements[i + 2].coordinate_index,
                face->face_elements[i + 2].texture_coordinate_index,
                face->face_elements[i + 2].normal_index);

            // float r = (float)rand() / (float)RAND_MAX;
            // float g = (float)rand() / (float)RAND_MAX;
            // float b = (float)rand() / (float)RAND_MAX;
            //
            // vertex1.r = r;
            // vertex1.g = g;
            // vertex1.b = b;
            //
            // vertex2.r = r;
            // vertex2.g = g;
            // vertex2.b = b;
            //
            // vertex3.r = r;
            // vertex3.g = g;
            // vertex3.b = b;

            dynamic_array_insert_end(&vertices, &vertex1);
            dynamic_array_insert_end(&vertices, &vertex2);
            dynamic_array_insert_end(&vertices, &vertex3);

            triangle_count++;
        }
    }

    struct model model;

    model.vertices = vertices.array;
    model.vertex_count = vertices.element_count;

    return model;
}

void model_draw(struct model *model, struct frame_buffer *frame_buffer) {
    renderer_draw_triangles(frame_buffer, model->vertices, model->vertex_count);
}
