#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include "lab_m1/tema 3/Lights/Cell.h"
#include "lab_m1/tema 3/Lights/Column.h"
#include "lab_m1/tema 3/Lights/Globe.h"
#include "lab_m1/tema 3/Lights/SpotLight.h"
#include "lab_m1/tema 3/Dancer.h"

namespace m1 {
	class Tema3 : public gfxc::SimpleScene {
	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;

		void CreateFloor();
		void CreateWalls();
		void CreateSpotLights();
		void CreateDancers();

		void DrawFloor();
		void DrawCeiling();
		void DrawWalls();
		void DrawGlobe();
		void DrawSpotLights();
		void DrawDancers();

		void UpdateDancers(float deltaTimeSeconds);
		
		void OnKeyPress(int key, int mods) override;

		void DrawMeshWithTexture(glm::mat4& modelMatrix, Mesh* mesh);

		std::vector< std::vector<LightCell*> > floor;

		std::vector<Column*> xOyColumsClose;
		std::vector<Column*> xOyColumsFar;
		std::vector<Column*> zOyColumsClose;
		std::vector<Column*> zOyColumsFar;

		std::vector<Spotlight*> spotlights;

		Globe* globe;

		bool floorIsActive;
		bool spotlightsAreActive;

		int gridW = 8;
		int gridH = 8;

		float height = 6.f;
		float width = gridW * 2.5f;
		float length = gridH * 2.5f;

		std::vector<Dancer*> dancers;

	public:
		Tema3();
		~Tema3();

		void Init();

	};
};