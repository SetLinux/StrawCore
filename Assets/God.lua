local returnTable = {}
local PlayerEnt = 1
local onSlopeRight = false
local onSlopeLeft = false
local SlopeDegree = 0
local StartPoint = XVector.new(0,0)
local DistanceinX = 0
local float XDirection = 0
local SlopeNormal = XVector.new(0,0,0)
function math.sign(x)
   if x<0 then
     return -1
   elseif x>0 then
     return 1
   else
     return 0
   end
end

local ClimbSlope = function (velocity,slopeAngle) 
	print("ClimbSlope")
	velocity = XVector.new(math.cos(SlopeDegree) * velocity.x,math.sin(SlopeDegree) * math.abs(velocity.x))
	--velocity = velocity * SlopeNormal
	return velocity
end



local DescendSlope = function (velocity,slopeAngle) 
	print("DescendSlope")
	velocity = XVector.new(math.cos(SlopeDegree) * velocity.x, velocity.y - math.sin(SlopeDegree) * math.abs(velocity.x))
	return velocity
end

local horiontalRayCast = function(normal, point , ent) 
	 
	DistanceinX = (StartPoint.x - point.x) * -1 
	SlopeNormal = normal
	SlopeDegree = math.acos(Dot(normal:Normal(),XVector.new(0,1,0):Normal())) 
	if(DistanceinX >0 )then
		onSlopeRight = true
		onSlopeLeft = false
	else
		onSlopeRight = false
		onSlopeLeft  = true
	end
	onSlope = true
end
returnTable["Init"] = function ()

end
returnTable["Update"] = function(dt)
	
end
returnTable["FixedUpdate"] = function (dt)

	--GetPhysicsComponent(1).velocity = XVector.new(0,0)	
	
	StartPoint = XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 50,GetPhysicsComponent(PlayerEnt).position.y - 50)
	PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 60,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)
	
	StartPoint =XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -50,GetPhysicsComponent(PlayerEnt).position.y - 50)

	PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -60,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)
	
	
	if(Input.isKeyDown(Keys["KEY_D"])) then
		if(onSlope) then
		GetPhysicsComponent(1).velocity = XVector.new(10,GetPhysicsComponent(1).velocity.y)

		GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x - DistanceinX,GetPhysicsComponent(1).velocity.y)	
		if(onSlopeRight) then
		GetPhysicsComponent(1).velocity = ClimbSlope(GetPhysicsComponent(1).velocity,slopeAngle)
	else
		GetPhysicsComponent(1).velocity = DescendSlope(GetPhysicsComponent(1).velocity,slopeAngle)
	end	
		GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x + DistanceinX,GetPhysicsComponent(1).velocity.y)	
	else
		GetPhysicsComponent(1).velocity = XVector.new(6,GetPhysicsComponent(1).velocity.y)
			
		end
	end
	if(Input.isKeyDown(Keys["KEY_A"])) then
	if(onSlope) then
		GetPhysicsComponent(1).velocity = XVector.new(-10,GetPhysicsComponent(1).velocity.y)

		GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x - DistanceinX,GetPhysicsComponent(1).velocity.y)	
			if(onSlopeLeft) then
		GetPhysicsComponent(1).velocity = ClimbSlope(GetPhysicsComponent(1).velocity,slopeAngle)
else
		GetPhysicsComponent(1).velocity = DescendSlope(GetPhysicsComponent(1).velocity,slopeAngle)
end		 GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x + DistanceinX ,GetPhysicsComponent(1).velocity.y)	
	
	else
	GetPhysicsComponent(1).velocity = XVector.new(-6,GetPhysicsComponent(1).velocity.y)
	end		
	end
	if(not onSlope) then
	GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x ,GetPhysicsComponent(1).velocity.y - .5)



end
onSlope = false

if(Input.isKeyDown(Keys["KEY_SPACE"])) then 
		GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x,20)	
	end
	
end

	
return returnTable
