#include "SimProject.h"

SimProject::SimProject(Canvas* canvas)
	: m_canvas(canvas) {}

void SimProject::Build() {
	// Generate digraph describing what nodes connect to what other nodes
	// Use digraph to instantiate and set connections between specific nodes such as source, server, sink, etc.

}

void SimProject::Run()
{
	RunSimulation();
}
