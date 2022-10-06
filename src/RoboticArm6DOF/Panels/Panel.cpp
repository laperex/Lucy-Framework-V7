#include "Panel.h"

bool lra::panel::PanelManager::IsHovering = false;

void lra::panel::PanelUpdate() {
	PanelManager::IsHovering = false;
	
	MainPanel();
	RendererPanel();
	RoboticArmPanel();
	AnimationPanel();
	SerialCommunicationPanel();
}
