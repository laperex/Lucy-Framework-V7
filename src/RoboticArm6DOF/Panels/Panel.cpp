#include "Panel.h"

void lra::panel::PanelUpdate() {
	MainPanel();
	RendererPanel();
	RoboticArmPanel();
	AnimationPanel();
	SerialCommunicationPanel();
	CanvasPanel();
}
