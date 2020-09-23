#include "autopan.hpp"

static AutoPan s_autopan;

static const float s_fs_recip = 1.f / 48000.f;

void MODFX_INIT(uint32_t platform, uint32_t api)
{
  (void)platform;
  (void)api;
  s_autopan.lfo.reset();
  s_autopan.lfo.setF0(1, s_fs_recip);
}

float scale_lfo(float val, float depth)
{
    return (((1 - depth) * val) / 1) + depth;
}

void MODFX_PROCESS(const float *main_xn, float *main_yn,
                     const float *sub_xn, float *sub_yn,
                     uint32_t frames)
{
    for (int i = 0; i < (frames * 2); i += 2) {
        float lfo_val = s_autopan.lfo.sine_uni();

        int left_ix = i;
        int right_ix = i + 1;

        const float d = s_autopan.params.depth;
        main_yn[left_ix] = main_xn[left_ix] * scale_lfo(lfo_val, d);
        main_yn[right_ix] = main_xn[right_ix] * scale_lfo(std::abs(lfo_val - 1), d);

        // Cycle LFO
        s_autopan.lfo.cycle();
    }
}

void MODFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_modfx_param_time:
    s_autopan.lfo.setF0(valf * 5 + 0.1, s_fs_recip);
    break;
  case k_user_modfx_param_depth:
    s_autopan.params.depth = std::abs(valf - 1);
    break;
  default:
    break;
  }
}
