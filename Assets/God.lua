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
    velocity = XVector.new(math.cos(slopeAngle) * velocity.x,math.sin(slopeAngle) * math.abs(velocity.x))
    --velocity = velocity * SlopeNormal
    return velocity
end



local DescendSlope = function (velocity,slopeAngle)
    velocity = XVector.new(math.cos(slopeAngle) * velocity.x, math.sin(slopeAngle) * (velocity.x))
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
    DistanceinX = 0
end
returnTable["Init"] = function ()

end
returnTable["Update"] = function(dt)

   --GetPhysicsComponent(1).velocity = XVector.new(0,0)
        onSlope = false
        StartPoint = XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 50,GetPhysicsComponent(PlayerEnt).position.y - 50)
        PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 53,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)

        StartPoint =XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -50,GetPhysicsComponent(PlayerEnt).position.y - 50)

        PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -53,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)


    if(Input.isKeyDown(Keys["KEY_D"])) then

        local hitpoint = XVector.new(0,0)    
        if(onSlope) then
   local canceled = false

    if(onSlopeRight)then
    

             PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x +60,900), XVector.new(GetPhysicsComponent(PlayerEnt).position.x +60,0),
                function(a,b,c) 

                hitpoint = b
                end)



             Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x+50, GetPhysicsComponent(1).position.y - 50)):Normal()

            else

             PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x -40,900), XVector.new(GetPhysicsComponent(PlayerEnt).position.x -40,0),
                function(a,b,c) 
                        
                hitpoint = b
                end)
 
                Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x-50, GetPhysicsComponent(1).position.y - 50)):Normal()
            
            end
         if(Input.isKeyDown(Keys["KEY_V"])) then  
                local Checker = {

                 Transform = { position = hitpoint, scale = XVector.new(50,50),rotation =0 }
                ,Sprite =
                 {texID = 1}}
                CreateEntity(Checker)
            end
   
            if(Distance(hitpoint,GetPhysicsComponent(1).position) > 100) then
                canceled = true
            end
            if(not canceled) then
            GetPhysicsComponent(1).velocity = Direction  * 16
            else 
            GetPhysicsComponent(1).velocity = XVector.new(16,GetPhysicsComponent(1).velocity.y) 
            end
        
        else
            GetPhysicsComponent(1).velocity = XVector.new(16,GetPhysicsComponent(1).velocity.y)

        end
    end
    if(Input.isKeyDown(Keys["KEY_A"])) then
        local hitpoint = XVector.new(0,0)    
 	     if(onSlope ) then
		    local Direction = 0
            local canceled = false
            if(onSlopeRight)then
    

             PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x +40,900), XVector.new(GetPhysicsComponent(PlayerEnt).position.x +40,0),
                function(a,b,c) 

                hitpoint = b
                end)



             Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x+50, GetPhysicsComponent(1).position.y - 50)):Normal()

			else

             PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x -55,900), XVector.new(GetPhysicsComponent(PlayerEnt).position.x -55,0),
                function(a,b,c) 
                        
                hitpoint = b
                end)
 
                Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x-50, GetPhysicsComponent(1).position.y - 50)):Normal()
            
            end
            if(Distance(hitpoint,GetPhysicsComponent(1).position) > 100) then
                canceled = true
            end
            if(Input.isKeyDown(Keys["KEY_V"])) then  
                local Checker = {

                 Transform = { position = hitpoint, scale = XVector.new(50,50),rotation =0 }
                ,Sprite =
                 {texID = 1}}
                CreateEntity(Checker)
            end
            if(not canceled) then
            GetPhysicsComponent(1).velocity = Direction  * 16
            else 
            GetPhysicsComponent(1).velocity = XVector.new(-16,GetPhysicsComponent(1).velocity.y) 
            end
        else
            GetPhysicsComponent(1).velocity = XVector.new(-16,GetPhysicsComponent(1).velocity.y) 
        end
    end
    
    --end
    --onSlope = false

--if(not onSlope) then
    GetPhysicsComponent(1).velocity  = XVector.new(GetPhysicsComponent(1).velocity.x , GetPhysicsComponent(1).velocity.y - 1.5)
--end
    if(Input.isKeyDown(Keys["KEY_SPACE"])) then
    local OnGround = false
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x-50,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x-50,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x+50,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x+50,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)


    if(OnGround) then GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x,35) end

end

end

returnTable["FixedUpdate"] = function (dt)

end


return returnTable
