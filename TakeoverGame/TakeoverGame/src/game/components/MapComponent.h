#pragma once

#include "../utils/Graph.h"

namespace tkv
{
	struct MapComponent : public Component<MapComponent>
	{
		MapComponent();
	};
	Graph* graph;

}
