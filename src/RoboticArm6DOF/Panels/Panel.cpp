#include "Panel.h"

bool lra::panel::PanelManager::IsHovering = false;

void lra::panel::PanelUpdate() {
	PanelManager::IsHovering = false;
	
	RendererPanel();
	RoboticArmPanel();
	AnimationPanel();
}
