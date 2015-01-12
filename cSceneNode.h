#pragma once

#include <vector>
#include "cMaths.h"
#include "cLogger.h"
#include "cRenderer.h"

class cSceneNode
{
private:
	JVector3 mPos;
	JVector3 mRot;
	vector<JVector3> mVerts;
	JVector3 mVertsCentre;
	bool mMovable;
	static const int mCollisionRadiusSqrd = 100;
	std::vector<cSceneNode*> mChildren;

	cLogger* mLog;
	cRenderer* mRen;
public:
	cSceneNode(void);
	~cSceneNode(void);

	bool AddChild(cSceneNode* node);
	bool DetachChild(cSceneNode* node);
	bool HasChild(const cSceneNode* child) const;
	bool CheckCollisions(const cSceneNode* compare_root) const;
	void UpdateTree(const JVector3& displacement, const float rotation, const cSceneNode* root) const;
	void Move(const JVector3& displacement, const cSceneNode* root);
	void Pivot(const float d, const JVector3& position, const cSceneNode* root);
	void Rotate(const float d, const cSceneNode* root);
	void Scale(float scale, cSceneNode* root);
	void Draw();

	void SetPosition(const JVector3& _pos);
	void SetX(int x);
	void SetY(int y);
	void SetMovable(bool movable);
	void SetVerts(JVector3* verts);
	bool SetVerts(char* filename);
	void CalculateVertsCentre();

	const JVector3& GetPosition() const;
	const JVector3& GetCentrePosition() const;
};

