RGB_MATRIX_EFFECT(jmw_glimmer)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include <math.h>

#define JMW_GLIMMER_BINS    110
#define JMW_GLIMMER_TPS     1000
#define JMW_GLIMMER_FPS     100
#define JMW_GLIMMER_FRAMES  (2 * JMW_GLIMMER_FPS)
#define JMW_GLIMMER_BASE_HUE_PERIOD 30
#define JMW_GLIMMER_MAX_TICKS (JMW_GLIMMER_FRAMES * JMW_GLIMMER_TPS / JMW_GLIMMER_FPS)

#if 0
static void jmw_hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b)
{
	float c = s * v;
	h = h < 1.f ? 6.f * h : 0.f; // handle h=0 <==> h=1
	switch ((int)h)
	{
		case 0: *r = c; *g = c * h; *b = 0; break;
		case 1: *r = c * (2 - h); *g = c; *b = 0; break;
		case 2: *r = 0; *g = c; *b = c * (h - 2); break;
		case 3: *r = 0; *g = c * (4 - h); *b = c; break;
		case 4: *r = c * (h - 4); *g = 0; *b = c; break;
		case 5: *r = c; *g = 0; *b = c * (6 - h); break;
	}
}
#endif

static void jmw_hsv8_to_rgb8(uint8_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b)
{
    HSV hsv = {h, s, v};
    RGB rgb = hsv_to_rgb(hsv);
    *r = rgb.r;
    *g = rgb.g;
    *b = rgb.b;
}

typedef struct jmw_glimmer_state
{
	uint16_t base_hue_timer;
	uint8_t base_hue;
	uint8_t frames_hsv[JMW_GLIMMER_FRAMES][3];

	uint16_t tick_counters[JMW_GLIMMER_BINS];
	uint8_t rgb[JMW_GLIMMER_BINS][3];
} jmw_glimmer_state;

static void jmw_glimmer_init(jmw_glimmer_state *st)
{
	st->base_hue_timer = 0;
	st->base_hue = 200;

	// Compute single key animation
	for (int i = 0; i < JMW_GLIMMER_FRAMES; i++)
	{
		float t = i * (1.f / JMW_GLIMMER_FPS);
		float intensity = 0.85f;

		// float val = expf(-2.f * t);
		float sin_value = sinf((float)M_PI * 1.5f * t);
		float sin2_value = sin_value * sin_value;
		float sin4_value = sin2_value * sin2_value;
		float val = sin4_value * sin4_value;

		st->frames_hsv[i][0] = intensity * val * (0.015f * 255.f);
		st->frames_hsv[i][1] = 255;
		st->frames_hsv[i][2] = 255 * ((1.f - intensity) + intensity * val);
	}

	// Randomize phases
	for (int i = 0; i < JMW_GLIMMER_BINS; i++)
		st->tick_counters[i] = rand() % JMW_GLIMMER_MAX_TICKS;

}

static bool jmw_glimmer(effect_params_t *params)
{
	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	static jmw_glimmer_state st;

	if (params->init)
		jmw_glimmer_init(&st);

	if (++st.base_hue_timer >= JMW_GLIMMER_BASE_HUE_PERIOD * JMW_GLIMMER_TPS / 256)
	{
		st.base_hue_timer = 0;
		st.base_hue++;
	}

	for (int i = 0; i < JMW_GLIMMER_BINS; i++)
	{
		int frame_num = st.tick_counters[i] * JMW_GLIMMER_FPS / JMW_GLIMMER_TPS;
		jmw_hsv8_to_rgb8(
			st.frames_hsv[frame_num][0] + st.base_hue,
			st.frames_hsv[frame_num][1],
			st.frames_hsv[frame_num][2],
			&st.rgb[i][0],
			&st.rgb[i][1],
			&st.rgb[i][2]
		);

		if (++st.tick_counters[i] >= JMW_GLIMMER_MAX_TICKS)
			st.tick_counters[i] = 0;
	}

	for (uint8_t i = led_min; i < led_max; i++)
	{
		uint8_t r = st.rgb[i % JMW_GLIMMER_BINS][0];
		uint8_t g = st.rgb[i % JMW_GLIMMER_BINS][1];
		uint8_t b = st.rgb[i % JMW_GLIMMER_BINS][2];
		rgb_matrix_set_color(i, r, g, b);
	}

	return rgb_matrix_check_finished_leds(led_max);
}

#endif
