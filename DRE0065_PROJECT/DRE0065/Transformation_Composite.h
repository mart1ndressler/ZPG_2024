#ifndef TRANSFORMATION_COMPOSITE_H
#define TRANSFORMATION_COMPOSITE_H

#include "transformation_component.h"
#include <vector>

using namespace glm;
using namespace std;

class TransformationComposite : public TransformationComponent
{
public:
    void addComponent(TransformationComponent* component);
    void apply(mat4& matrix) const override;
    ~TransformationComposite();

private:
    vector<TransformationComponent*> components;
};
#endif