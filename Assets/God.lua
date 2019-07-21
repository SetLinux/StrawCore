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
local Direction = XVector.new(0,0,0)
local toBeMoved = XVector.new(0,0,0)
local canceled = false
local Handled = false
local State = {Standing = "Standing",MoveRight="MoveRight",MoveLeft="MoveLeft",airbrone="airbrone"}
local PlayerState = State.Standing
local Handle = false
local ignore = false
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

local horiontalRayCast = function(normal, point , ent)

    if(GetPhysicsComponent(ent).slope) then
       
       local bSlopeDegree = (math.atan2(normal:Normal().y,normal:Normal().x) - math.atan2(1,0))
        if(bSlopeDegree * (180 / 3.14)  > -70 and bSlopeDegree * (180 / 3.14) < 70) then
        SlopeDegree = bSlopeDegree
        SlopeEnt = ent
        DistanceinX = (StartPoint.x - point.x) * -1

        SlopeNormal = normal
        --signed_angle = atan2(b.y,b.x) - atan2(a.y,a.x)
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
end

end

PhysicsSystem.PreSolve = function(normal,ent1,ent2,contact)
if(GetPhysicsComponent(ent2).slope)then
    contact:setEnabled(false)
    Handle = true
    onSlope = true
    print("GOT THE HIT BUT CAN'T REACT")
    end
end
function math.cross(a,b)
--→a×→b=⟨a2b3−a3b2,a3b1−a1b3,a1b2−a2b1⟩

return XVector.new(a.y * b.z - a.z * b.y , a.z*b.x - a.x * b.z , a.x * b.y - a.y * b.x)
end
local difference = 2
returnTable["Init"] = function ()

end
local counterf = 0
returnTable["Update"] = function(dt)
  
end

returnTable["FixedUpdate"] = function (dt)
    PlayerState = State.Standing
    Handle = false
    if(Input.isKeyDown(Keys["KEY_D"])) then
        PlayerState = State.MoveRight
    end
    if(Input.isKeyDown(Keys["KEY_A"])) then
        PlayerState = State.MoveLeft
    end
    local PlayerLocation = GetPhysicsComponent(PlayerEnt).position
    onSlope = false
    StartPoint = XVector.new(PlayerLocation.x ,PlayerLocation.y -50) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x + 70,PlayerLocation.y - 50), horiontalRayCast)

    StartPoint = XVector.new(PlayerLocation.x,PlayerLocation.y -50) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x - 70,PlayerLocation.y - 50), horiontalRayCast)
    StartPoint = XVector.new(PlayerLocation.x + 50,PlayerLocation.y) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x + 50,PlayerLocation.y - 51), horiontalRayCast)

    StartPoint = XVector.new(PlayerLocation.x - 50,PlayerLocation.y) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x - 50,PlayerLocation.y - 51), horiontalRayCast)

end
returnTable["LateFixedUpdate"] = function (dt)
if(not onSlope) then
        GetPhysicsComponent(PlayerEnt).velocity = XVector.new(0,GetPhysicsComponent(PlayerEnt).velocity.y)

    if(PlayerState == State.MoveRight) then

    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(5,GetPhysicsComponent(PlayerEnt).velocity.y)
    elseif(PlayerState == State.MoveLeft) then
    
    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(-5,GetPhysicsComponent(PlayerEnt).velocity.y)
    elseif(PlayerState == State.Standing) then

    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(0,GetPhysicsComponent(PlayerEnt).velocity.y)
    end

    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(GetPhysicsComponent(PlayerEnt).velocity.x,GetPhysicsComponent(PlayerEnt).velocity.y - 3.5)

end
if(onSlope) then
    local PlayerLocation = GetPhysicsComponent(PlayerEnt).position
    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(0,0)
    if(PlayerState == State.MoveRight) then
    PhysicsSystem.rayCast(XVector.new(PlayerLocation.x + 50 + GetClimbX(10,45),PlayerLocation.y + 75),XVector.new(PlayerLocation.x + 50 + GetClimbX(10,45),0),function(normal,point,ent)    GetPhysicsComponent(PlayerEnt).velocity = (point - XVector.new(PlayerLocation.x+50,PlayerLocation.y-50)) 
 end)
    
    elseif(PlayerState == State.MoveLeft) then
    PhysicsSystem.rayCast(XVector.new(PlayerLocation.x + 50 - GetClimbX(10,45) ,PlayerLocation.y + 75),XVector.new(PlayerLocation.x + 50 - GetClimbX(10,45),0),function(normal,point,ent) 

        GetPhysicsComponent(PlayerEnt).velocity = (point - XVector.new(PlayerLocation.x+50,PlayerLocation.y-50)) 


    end)
    
    elseif(PlayerState == State.Standing) then
        PhysicsSystem.rayCast(XVector.new(PlayerLocation.x + 50,PlayerLocation.y + 75),XVector.new(PlayerLocation.x + 50,0),function(normal,point,ent)    GetPhysicsComponent(PlayerEnt).velocity = (point - XVector.new(PlayerLocation.x+50,PlayerLocation.y-50)) 
 end)
    end
end
print(GetPhysicsComponent(PlayerEnt).position)
end
return returnTable
