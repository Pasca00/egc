#include "lab_m1/tema 3/Lights/Column.h"

Column::Column(glm::vec3 pos, glm::vec3 size, LightCell* correspondingCell) {
	this->pos = pos;
	this->size = size;
	this->correspondingCell = correspondingCell;
}