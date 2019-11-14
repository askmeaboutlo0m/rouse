typedef struct R_Canvas R_Canvas;

R_FrameBufferOptions R_canvas_frame_buffer_options(int width, int height);

R_Canvas *R_canvas_new(int width, int height);

void R_canvas_free(R_Canvas *canvas);

R_Sprite *R_canvas_sprite(R_Canvas *canvas);

void R_canvas_render(R_Canvas *canvas, R_Nvg *nvg, int target_width,
                     int target_height);
