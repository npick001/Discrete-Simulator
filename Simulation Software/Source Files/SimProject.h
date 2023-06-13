#pragma once

#include "Canvas.h"

class SimProject {
private:
	Canvas* m_canvas;
public:
	SimProject(Canvas* canvas);

	void Build();
};