#pragma once

#include "Animator.h"

namespace lra {
	void TraceAnimationPoints(int start_idx, int idx, AnimationProperty* animation, const glm::vec4& color = { 1, 1, 0, 1 });
	void Trace(AnimationProperty* animation, int idx, float progress, bool show_progress, const glm::vec4& color = { 1, 1, 0, 1 });
}