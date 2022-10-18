#pragma once

namespace lra::panel {
	#define WindowAlpha (0.8)

	void MainPanel();
	void RendererPanel();
	void RoboticArmPanel();
	void PropertiesPanel();
	void AnimationPanel();
	void SerialCommunicationPanel();
	void CanvasPanel();
	void ComputerVisionPanel();

	void PanelUpdate();
}