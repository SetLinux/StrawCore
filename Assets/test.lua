entity = {
	ComponentA = {},
	ComponentB = {}
}

local PlayerEntity = 0
local RAYCASTSHIT = function (entt) 
	
	--GetPhysicsComponent(PlayerEntity).velocity = XVector.new(GetPhysicsComponent(PlayerEntity).velocity.x,-GetPhysicsComponent(PlayerEntity).velocity.y)
	GetPhysicsComponent(PlayerEntity).velocity = XVector.new(0,30)
end
local tex = 0
local Player = {}
local returnTable = {}
returnTable["Init"] = function ()
	tex = MakeTexture("sheet.png")
	for i=1,40 do
		local floor = {
			Transform = {
				position = XVector.new(i * 50,0),
				scale = XVector.new(50,50),
				rotation = 0
			},
			Sprite = {
				texID = tex
			}
		}
		CreateEntity(floor)
	end
		local floorer = {
			Transform = {
				position = XVector.new(0,0),
				scale = XVector.new(50,50),
				rotation = 0
			},
			Physics = {
				position = XVector.new(0,0),
				scale = XVector.new(2000,0),
				type = "Static"
			}
		}
		CreateEntity(floorer)
	
	Player = {
		Transform = {
			position = XVector.new(500,500),
			scale = XVector.new(100,100),
			rotation = 0
		},
		Sprite = {
			texID = tex
		},
		Physics = {
			position = XVector.new(500,500),
			scale = XVector.new(100,100);
		}
	}
	PlayerEntity =  CreateEntity(Player)
	print(PlayerEntity)
	end
returnTable["Update"] = function (dt) 
	GetTransformComponent(PlayerEntity).isDirty = true
	if(Input.isKeyDown(Keys["KEY_D"])) then
		GetPhysicsComponent(PlayerEntity).velocity = XVector.new(10,GetPhysicsComponent(PlayerEntity).velocity.y)
	end
	if(Input.isKeyDown(Keys["KEY_A"])) then
		GetPhysicsComponent(PlayerEntity).velocity = XVector.new(-10,GetPhysicsComponent(PlayerEntity).velocity.y)
	end
		PhysicsSystem.rayCast(GetPhysicsComponent(PlayerEntity).position , XVector.new(GetPhysicsComponent(PlayerEntity).position.x, GetPhysicsComponent(PlayerEntity).position.y - 55) , RAYCASTSHIT ) 
	--if(Input.isKeyDown(Keys["KEY_SPACE"]))then
	--	PhysicsSystem.rayCast(GetPhysicsComponent(PlayerEntity).position , XVector.new(GetPhysicsComponent(PlayerEntity).position.x, GetPhysicsComponent(PlayerEntity).position.y - 55) , RAYCASTSHIT ) 
	--end
end

returnTable["FixedUpdate"] = function (dt)

	
end

return returnTable
