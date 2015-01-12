	
	if(left trigger down)
		{
		if(left hand not already holding)
		{
			Grab(hand position);
		}
		//else contunue to hold
	}
	if(left trigger up)
	{
		handNode->RemoveParent();
	}

	if(right trigger down)
	{
		if(right hand not already holding)
		{
			Grab(hand position);
		}
		//else contunue to hold
	}
	if(right trigger up)
	{
		handNode->RemoveParent();
	}

	Grab(position hand pos)
	{
		sceneNode = check collision(hand pos,root node);
		if(sceneNode)
		{
			sceneNode->AddChild(hand,hand pos);
			handNode->AddParent(sceneNode);
		}
	}
	