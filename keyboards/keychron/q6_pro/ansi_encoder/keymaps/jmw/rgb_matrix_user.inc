RGB_MATRIX_EFFECT(jmw_glimmer)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include <math.h>

#define JMW_GLIMMER_BINS 110
#define JMW_GLIMMER_TPS  1000

struct glimmer_state {
	uint16_t ticks;
	float t;
	float phases[JMW_GLIMMER_BINS];
	uint8_t rgb[JMW_GLIMMER_BINS][3];
};

static void jmw_hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b)
{
	float c = s * v;
	h = h < 1.f ? 6.f * h : 0.f; // handle h=0 <==> h=1
	switch ((int)h)
	{
		case 0: *r = c; *g = h; *b = 0; break;
		case 1: *r = 2 - h; *g = c; *b = 0; break;
		case 2: *r = 0; *g = c; *b = h - 2; break;
		case 3: *r = 0; *g = 4 - h; *b = c; break;
		case 4: *r = h - 4; *g = 0; *b = c; break;
		case 5: *r = c; *g = 0; *b = 6 - h; break;
	}
}

static bool jmw_glimmer(effect_params_t *params)
{
	static struct glimmer_state st;
	if (params->init)
	{
		st = (struct glimmer_state){
			.ticks = 0,
			.t = 0.f,
		};

		for (int i = 0; i < JMW_GLIMMER_BINS; i++)
			st.phases[i] = rand() * (1.f / RAND_MAX);
	}

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	st.t = st.ticks * (1.f / JMW_GLIMMER_TPS);
	if (++st.ticks == JMW_GLIMMER_TPS)
		st.ticks = 0;

	for (int i = 0; i < JMW_GLIMMER_BINS; i++)
	{
		float sin_value = sinf(2.f * (float)M_PI * (1.5f * st.t - st.phases[i]));
		float sin2_value = sin_value * sin_value;
		float sin4_value = sin2_value * sin2_value;
		float offset = sin4_value * sin4_value;

		float base_hue = 0.75f;
		float intensity = 0.85f;

		float h = fmodf(base_hue + offset * intensity * 0.01f, 1.f);
		float s = 1.f;
		float v = (1.f - intensity) + intensity * offset;
		float r = 0.f, g = 0.f, b = 0.f;

		jmw_hsv_to_rgb(h, s, v, &r, &g, &b);
		st.rgb[i][0] = r * 255;
		st.rgb[i][1] = g * 255;
		st.rgb[i][2] = b * 255;
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
