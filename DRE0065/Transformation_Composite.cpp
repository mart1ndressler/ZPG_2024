#include "transformation_composite.h"

void TransformationComposite::addComponent(TransformationComponent* component) {components.push_back(component);}
void TransformationComposite::apply(mat4& matrix) const {for(const auto& component : components) component->apply(matrix);}
TransformationComposite::~TransformationComposite() {for(auto component : components) delete component;}