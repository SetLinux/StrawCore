local returnTable = {}
local onSlope = false
local PlayerEnt = 1
local onSlopeRight = false
local onSlopeLeft = false
local SlopeDegree = 0
local StartPoint = XVector.new(0,0)
local DistanceinX = 0
local float XDirection = 0
local SlopeNormal = XVector.new(0,0,0)
local SlopeEnt = 0
local State = {Standing = "Standing",MoveRight="MoveRight",MoveLeft="MoveLeft",airbrone="airbrone"}
local PlayerState = State.Standing
local PlayerScale = XVector.new(100,100,0)
local HorizontalRayCount = 3
local VerticalRayCount = 3
local HorizontalRaySpacing = 0
local VerticalRaySpacing = 0
local bounds = {}
local Player = {velocity = XVector.new(0,0)}
function math.sign(x)
    if x<0 then
        return -1
    elseif x>0 then
        return 1
    else
        return 0
    end
end
local GetClimbX = function(xval,slopeAngle)

    return math.cos(slopeAngle) * xval

end

local GetClimbY = function(yval,slopeAngle)
    return math.sin(slopeAngle) * yval
end

local normaltoDegree = function(normal)
       return (math.atan2(normal:Normal().y,normal:Normal().x) - math.atan2(1,0)) * (180/3.14)

end
returnTable["Init"] = function ()

end
returnTable["Update"] = function(dt)
  end

returnTable["FixedUpdate"] = function (dt)
Player.velocity.y = -5
Player.velocity.x = 0
local test = {Transform = {position = XVector.new(GetPhysicsComponent(1).position.x+ 25,GetPhysicsComponent(1).position.y - 25),scale = XVector.new(10,10),rotation = 0},Sprite = {texID = 1}}
        --CreateEntity(test)

if(Input.isKeyDown(Keys["KEY_D"])) then
    local handled = false
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ 25,GetPhysicsComponent(1).position.y - 25),XVector.new(1,0,0),10,function(normal1,point,ent) 
        if(GetPhysicsComponent(ent).slope ) then
       PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + 40,900 + 25),XVector.new(0,-1,0),100000,function(normal2,point,ent) 
            local Direction = point - XVector.new(GetPhysicsComponent(1).position.x+25/2,GetPhysicsComponent(1).position.y-25/2)
         
            if(normal1 ~= normal2)then
                Player.velocity = XVector.new(GetClimbX(4,Radians(45)),GetClimbY(4,Radians(45)))
            else
            Player.velocity = Direction
            end
            handled = true        
    end)
end
    end)
    if(not handled) then
    Player.velocity.x = 10
    end
end
if(Input.isKeyDown(Keys["KEY_A"])) then
    Player.velocity.x = -10
end


end
returnTable["LateFixedUpdate"] = function (dt)
if(Player.velocity.y~=0) then
PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(0,Player.velocity.y))

end
if(Player.velocity.x~=0) then
PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(Player.velocity.x,0))    
end
end
return returnTable
