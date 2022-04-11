#include <unordered_map>

#include "core/window/window_object.h"
#include "core/gpu/mesh.h"
#include "lab_m1/tema 2/Camera/MyCamera.h"
#include "lab_m1/tema 2/Player/Bullet.h"
#include "lab_m1/tema 2/Player/ResourceContainer.h"
#include "lab_m1/tema 2/Map/TileMap.h"

using namespace std;

class Player {
	private:
		Hitbox* hitbox;
		float moveSpeed;

		float fireRate;
		float timeSinceLastBullet;

		ResourceContainer* health;
		ResourceContainer* escapeTimer;

		std::vector<std::pair<Hitbox*, glm::vec3>> bodyParts;

		void createMesh(unordered_map<string, Mesh*>& meshes, unordered_map<string, Texture2D*>& textures, string& path, string& texturePath);

	public:
		Player(float x, float y, unordered_map<string, Mesh*>& meshes, unordered_map<string, Texture2D*>& texures, string& path, string& texturePath);
		void handleInput(float deltaTimeSeconds, WindowObject* window , MyCamera* camera, vector<Bullet*>& bullets, TileMap* tileMap);

		Hitbox* getHitbox();

		std::vector<std::pair<Hitbox*, glm::vec3>> getBodyParts();

		ResourceContainer* getHealth();
		ResourceContainer* getTimer();

		bool collidesFront;
		bool collidesBack;
		bool collidesRight;
		bool collidesLeft;
};