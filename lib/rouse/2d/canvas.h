typedef struct R_Canvas R_Canvas;

R_Canvas *R_canvas_new(int width, int height);

void R_canvas_free(R_Canvas *canvas);

R_Sprite *R_canvas_sprite(R_Canvas *canvas);

R_FrameBuffer *R_canvas_render(R_Canvas *canvas, NVGcontext *vg);
