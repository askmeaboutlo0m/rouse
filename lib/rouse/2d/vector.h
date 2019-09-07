typedef struct R_VectorImage R_VectorImage;


R_VectorImage *R_vector_image_new(const char *title, R_ParseReadFn read,
                                  R_UserData user, int bufsize,
                                  unsigned char buffer[static bufsize]);

R_VectorImage *R_vector_image_from_file(const char *path);

void R_vector_image_free(R_VectorImage *vi);

int R_vector_image_width (R_VectorImage *vi);
int R_vector_image_height(R_VectorImage *vi);

void R_vector_image_draw(R_VectorImage *vi, NVGcontext *vg,
                         const float parent_matrix[static 6]);
