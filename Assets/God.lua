local returnTable = {}
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
    difference = GetClimbX(20,SlopeDegree)
    counterf = counterf + 1
   onSlope = false
    
    StartPoint = XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 50,GetPhysicsComponent(PlayerEnt).position.y - 50)
    PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + 51,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)

    StartPoint =XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -50,GetPhysicsComponent(PlayerEnt).position.y - 50)

    PhysicsSystem.rayCast(StartPoint, XVector.new(GetPhysicsComponent(PlayerEnt).position.x + -51,GetPhysicsComponent(PlayerEnt).position.y - 50),horiontalRayCast)
    if(not onSlope)then
       GetPhysicsComponent(1).velocity = XVector.new(0,GetPhysicsComponent(1).velocity.y)
    else
        GetPhysicsComponent(1).velocity = XVector.new(0,0)
    end


    if(Input.isKeyDown(Keys["KEY_D"])) then

        local hitpoint = XVector.new(0,0)
        if(onSlope) then
            local canceled = false
            local hitternormal = XVector.new(0,0,0)

            if(onSlopeRight)then

                PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x +50 + difference,GetPhysicsComponent(PlayerEnt).position.y), XVector.new(GetPhysicsComponent(PlayerEnt).position.x +50 + difference,0),
                    function(a,b,c)
                        hitternormal = a
                        if(a ~= SlopeNormal)then
                            canceled=true end
                        hitpoint = b
                    end)

                if(not canceled) then

                    Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x+50, GetPhysicsComponent(1).position.y - 50))
                end
            else

                PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x - ( 50 - difference) ,GetPhysicsComponent(PlayerEnt).position.y), XVector.new(GetPhysicsComponent(PlayerEnt).position.x - ( 50 - difference),0),
                    function(a,b,c)
                        --      if(a ~= SlopeNormal)then  canceled=true end
                        hitternormal = a
                        hitpoint = b
                    end)

                Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x-50, GetPhysicsComponent(1).position.y - 50))

            end

            if(Distance(hitpoint,GetPhysicsComponent(1).position) > 100) then
                canceled = true
            end
            if(not canceled) then

                GetPhysicsComponent(1).velocity = Direction
            else


                --   GetPhysicsComponent(1).velocity = Direction
                GetPhysicsComponent(1).velocity = XVector.new(GetClimbX(20,SlopeDegree),GetClimbY(20,SlopeDegree))

                --GetPhysicsComponent(1).velocity = Direction
            end

        else
            GetPhysicsComponent(1).velocity = XVector.new(20,GetPhysicsComponent(1).velocity.y)

        end
    end
    if(Input.isKeyDown(Keys["KEY_A"])) then
        local hitpoint = XVector.new(0,0)
        if(onSlope ) then
            local canceled = false
            if(onSlopeRight)then


                PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x + ( 50 - difference),GetPhysicsComponent(PlayerEnt).position.y), XVector.new(GetPhysicsComponent(PlayerEnt).position.x + ( 50 - difference),0),
                    function(a,b,c)
                        --                  if(a ~= SlopeNormal)then  canceled=true end
                        hitpoint = b
                    end)

            
                    Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x+50, GetPhysicsComponent(1).position.y - 50))
            
            else

                PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x -( 50 + difference),GetPhysicsComponent(PlayerEnt).position.y), XVector.new(GetPhysicsComponent(PlayerEnt).position.x -( 50 + difference),0),
                    function(a,b,c)

                        if(a ~= SlopeNormal)then   canceled=true end
                        hitpoint = b
                    end)
                Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x-50, GetPhysicsComponent(1).position.y - 50))

            end
            if(Distance(hitpoint,GetPhysicsComponent(1).position) > 100) then
                canceled = true

                --More Work Shit
                local bhitpoint = XVector.new(0,0,0)
                PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x + ( 50 - 1),GetPhysicsComponent(PlayerEnt).position.y), XVector.new(GetPhysicsComponent(PlayerEnt).position.x + ( 50 - 1),0),
                    function(a,b,c)
                        --                  if(a ~= SlopeNormal)then  canceled=true end
                        bhitpoint = b
                    end)
                Direction = ( hitpoint - XVector.new(GetPhysicsComponent(1).position.x+50, GetPhysicsComponent(1).position.y - 50))
            




            
            end
            if(Input.isKeyDown(Keys["KEY_V"])) then
                local Checker = {

                        Transform = { position = hitpoint, scale = XVector.new(5,5),rotation =0 }
                        ,Sprite =
                        {texID = 1}}
                CreateEntity(Checker)
            end
            if(not canceled) then
            
                         GetPhysicsComponent(1).velocity = Direction
   else
                GetPhysicsComponent(1).velocity = XVector.new(GetClimbX(-20,SlopeDegree),GetClimbY(-20,SlopeDegree) )
            end
        else
                GetPhysicsComponent(1).velocity = XVector.new(-20,GetPhysicsComponent(1).velocity.y)
       end
    end

    --end
    --onSlope = false
    if(not onSlope) then
        GetPhysicsComponent(1).velocity  = XVector.new(GetPhysicsComponent(1).velocity.x , GetPhysicsComponent(1).velocity.y - 3.5)
    end
    if(Input.isKeyDown(Keys["KEY_SPACE"])) then
        local OnGround = false
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x-50,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x-50,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(PlayerEnt).position.x+50,GetPhysicsComponent(PlayerEnt).position.y-50), XVector.new(GetPhysicsComponent(PlayerEnt).position.x+50,GetPhysicsComponent(PlayerEnt).position.y-56),function(a,b,c)  OnGround = true end)


       if(OnGround) then GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x,60) end
---GetPhysicsComponent(1).velocity = XVector.new(GetPhysicsComponent(1).velocity.x,60)
    end
    print(GetPhysicsComponent(1).velocity)
end

returnTable["FixedUpdate"] = function (dt)

end


return returnTable
