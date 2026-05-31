#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/io.h"
#include "data_structures/dynamic_array.h"
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

struct face_element parse_face_element(char *string) {
    struct face_element element = {0, 0, 0};

    char *index = strsep(&string, "/");
    element.coordinate_index = atoi(index);

    index = strsep(&string, "/");

    if (index) {
        element.texture_coordinate_index = atoi(index);
    }

    index = strsep(&string, "/");

    if (index) {
        element.normal_index = atoi(index);
    }

    return element;
}

struct face parse_face(char *line) {
    char *pointer = line + 2;

    struct dynamic_array face_elements;
    dynamic_array_init(&face_elements, sizeof(struct face_element));

    char *element = strtok(pointer, " ");

    while (element != NULL) {
        struct face_element new_element = parse_face_element(element);
        dynamic_array_insert_end(&face_elements, &new_element);

        element = strtok(NULL, " ");
    }

    struct face face;

    face.face_elements = face_elements.array;
    face.face_element_count = face_elements.element_count;

    return face;
}

struct vertex
construct_vertex(struct vertex_coordinate *coordinate,
                 struct vertex_texture_coordinate *texture_coordinate,
                 struct vertex_normal *normal) {
    struct vertex vertex;

    vertex.x = coordinate->x;
    vertex.y = coordinate->y;
    vertex.z = coordinate->z;

    vertex.normal_x = normal->x;
    vertex.normal_y = normal->y;
    vertex.normal_z = normal->z;

    vertex.texture_x = texture_coordinate->x;
    vertex.texture_y = texture_coordinate->y;

    vertex.r = normal->x;
    vertex.g = normal->y;
    vertex.b = normal->z;

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
            struct face face = parse_face(line);
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
                dynamic_array_get(&coordinates,
                                  face->face_elements[i].coordinate_index - 1),
                dynamic_array_get(
                    &texture_coordinates,
                    face->face_elements[i].texture_coordinate_index - 1),
                dynamic_array_get(&normals,
                                  face->face_elements[i].normal_index - 1));

            struct vertex vertex2 = construct_vertex(
                dynamic_array_get(&coordinates,
                                  face->face_elements[i + 1].coordinate_index -
                                      1),
                dynamic_array_get(
                    &texture_coordinates,
                    face->face_elements[i + 1].texture_coordinate_index - 1),
                dynamic_array_get(&normals,
                                  face->face_elements[i + 1].normal_index - 1));

            struct vertex vertex3 = construct_vertex(
                dynamic_array_get(&coordinates,
                                  face->face_elements[i + 2].coordinate_index -
                                      1),
                dynamic_array_get(
                    &texture_coordinates,
                    face->face_elements[i + 2].texture_coordinate_index - 1),
                dynamic_array_get(&normals,
                                  face->face_elements[i + 2].normal_index - 1));

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

void model_to_buffers(struct model *model, struct vertex **vertices,
                      unsigned int *vertex_count) {
    // *vertex_count = model->face_count * 3;
    // *vertices = malloc(*vertex_count * sizeof(struct vertex));
    //
    // for (int f = 0; f < model->face_count; f++) {
    //     for (int i = 0; i < 3; i++) {
    //         struct vertex_coordinate *coordinate =
    //             &model->coordinates
    //                  [model->faces[f].face_elements[i].coordinate_index];
    //
    //         printf("%d\n", f * 3 + i);
    //         // vertices[f * 3 + i]->x = coordinate->x;
    //         // vertices[f * 3 + i]->y = coordinate->y;
    //         // vertices[f * 3 + i]->z = coordinate->z;
    //         printf("%f, %f, %f\n", coordinate->x, coordinate->y,
    //         coordinate->z);
    //     }
    // }
}
