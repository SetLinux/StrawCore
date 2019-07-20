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
local State = {Standing = "Standing",MoveRight="MoveRight",MoveLeft="MoveLeft"}
local PlayerState = State.Standing
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

local GetClimbY = function(yval,slopeAngle)
    return math.sin(slopeAngle) * yval
end
local GetClimbX = function(xval,slopeAngle)

    return math.cos(slopeAngle) * xval

end
local handleASlope = function (normal,difference,SlopeEnt) 
     Handled = true
     print((math.atan2(normal:Normal().y,normal:Normal().x) - math.atan2(1,0)) *  (180 / 3.14) )
     local PlayerLocation = GetPhysicsComponent(PlayerEnt).position
        if(PlayerState == State.Standing) then
            local CollisionPoint = XVector.new(0,0,0)
            PhysicsSystem.rayCast(XVector.new(PlayerLocation.x+50,PlayerLocation.y),XVector.new(PlayerLocation.x+50,0),function ( a,b,c )
                CollisionPoint = b
            end)
        GetPhysicsComponent(1).velocity = XVector.new(0,0,0)

            toBeMoved = XVector.new(CollisionPoint.x - 50,CollisionPoint.y + 50)
        elseif(PlayerState == State.MoveRight) then
            local CollisionPoint = XVector.new(0,0,0)
            PhysicsSystem.rayCast(XVector.new(PlayerLocation.x+50 + difference,PlayerLocation.y),XVector.new(PlayerLocation.x+50 + difference,0),function ( a,b,c )
                CollisionPoint = b
            end)
            GetPhysicsComponent(PlayerEnt).velocity = XVector.new(0,0)

            toBeMoved = XVector.new(CollisionPoint.x - 50,CollisionPoint.y + 50)
        elseif(PlayerState == State.MoveLeft) then
            local CollisionPoint = XVector.new(0,0,0)
            local CollisionNormal = XVector.new(0,0,0)
            local CollisionEntity = -1
            PhysicsSystem.rayCast(XVector.new(PlayerLocation.x+50 - difference,PlayerLocation.y),XVector.new(PlayerLocation.x+50 - difference,0),function ( a,b,c )
                CollisionPoint = b
          
            end)
            toBeMoved = XVector.new(CollisionPoint.x - 50,CollisionPoint.y + 50)
            end
        
end
PhysicsSystem.PreSolve = function(normal,ent1,ent2,contact)
    if(GetPhysicsComponent(ent2).slope and onSlope) then
        contact:setEnabled(false)
         
        handleASlope(normal,GetClimbX(10,45),ent2)
        end
    end

function math.cross(a,b)
--→a×→b=⟨a2b3−a3b2,a3b1−a1b3,a1b2−a2b1⟩

return XVector.new(a.y * b.z - a.z * b.y , a.z*b.x - a.x * b.z , a.x * b.y - a.y * b.x)
end
local difference = 2
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

returnTable["Init"] = function ()

end
local counterf = 0
returnTable["Update"] = function(dt)
  
end

returnTable["FixedUpdate"] = function (dt)
    PlayerState = State.Standing
    local PlayerLocation = GetPhysicsComponent(1).position
    ignore = false
    local CollisionEntity = -1
    
    if(onSlope) then
    PhysicsSystem.rayCast(XVector.new(PlayerLocation.x-50 - GetClimbX(10,45),PlayerLocation.y),XVector.new(PlayerLocation.x-50 - GetClimbX(10,45),PlayerLocation.y-51),function ( a,b,c )
                CollisionEntity = c
            end)
            if(CollisionEntity ~= -1 and CollisionEntity ~= SlopeEnt) then
                onSlope     =false 
                GetPhysicsComponent(1).velocity =XVector.new(-20,0)
 end            
end
    onSlope = false
    
    Handled = false
    
    if(Input.isKeyDown(Keys["KEY_D"])) then
        PlayerState = State.MoveRight
    end
    if(Input.isKeyDown(Keys["KEY_A"])) then
        PlayerState = State.MoveLeft
    end
    StartPoint = XVector.new(PlayerLocation.x  - 49 ,PlayerLocation.y -50) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x + 51,PlayerLocation.y - 50), horiontalRayCast)

    StartPoint = XVector.new(PlayerLocation.x + 49,PlayerLocation.y -50) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x - 51,PlayerLocation.y - 50), horiontalRayCast)


    StartPoint = XVector.new(PlayerLocation.x + 50,PlayerLocation.y- 49) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x + 50,PlayerLocation.y - 51), horiontalRayCast)

    StartPoint = XVector.new(PlayerLocation.x - 50,PlayerLocation.y + 49) 
    PhysicsSystem.rayCast(StartPoint,XVector.new(PlayerLocation.x - 50,PlayerLocation.y - 51), horiontalRayCast)
    
end
returnTable["LateFixedUpdate"] = function (dt)
    if(onSlope and not Handled) then
            handleASlope(XVector.new(1,1,0),GetClimbX(10,45),3)
       
        end
 if(not onSlope) then

    local PlayerVel = GetPhysicsComponent(PlayerEnt).velocity
    local PlayerLocation = GetPhysicsComponent(PlayerEnt).position
    GetPhysicsComponent(1).velocity = XVector.new(0,0)
    PlayerVel.x = 0
    if(PlayerState == State.MoveRight) then
        GetPhysicsComponent(PlayerEnt).velocity = XVector.new(30,PlayerVel.y)
        local AComingSlope = false
        local SlopeENT = 0
        PlayerVel.x = 20
    elseif(PlayerState == State.MoveLeft) then
        GetPhysicsComponent(PlayerEnt).velocity = XVector.new(-10,PlayerVel.y)
        PlayerVel.x  = -20
            end
    GetPhysicsComponent(PlayerEnt).velocity = XVector.new(PlayerVel.x,PlayerVel.y - 1.5)
    
  else
    GetPhysicsComponent(PlayerEnt).position = toBeMoved
--     GetPhysicsComponent(PlayerEnt).velocity = XVector.new(-30,0)
             
  end
end


return returnTable
