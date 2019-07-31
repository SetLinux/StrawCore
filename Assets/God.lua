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
local Descending = false
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
local handleMovment = function()

    if(Descending) then
        if(Player.velocity.x~=0) then
            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(Player.velocity.x,0),false)
        end

        if(Player.velocity.y~=0) then

            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(0,Player.velocity.y),false)

        end

    else
        if(Player.velocity.y~=0) then
            if(PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(0,Player.velocity.y),false)) then

                Player.velocity.y = 0
            end
        end
        if(Player.velocity.x~=0) then
            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(Player.velocity.x,0),false)
        end
    end
end
returnTable["Init"] = function ()

end
returnTable["Update"] = function(dt)
end

returnTable["FixedUpdate"] = function (dt)
    Descending = false
    --Player.velocity.y = 0
    onSlopeRight = false
    onSlopeLeft = false
    local SlopeSpeed = 16
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ 23,GetPhysicsComponent(1).position.y - 25),XVector.new(1,0,0),4,function(normal1,point,ent)
        if(GetPhysicsComponent(ent).slope) then
        onSlopeRight = true
        SlopeNormal = normal1
    end
    end)
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- 23,GetPhysicsComponent(1).position.y - 25),XVector.new(-1,0,0),4,function(normal1,point,ent)
        if(GetPhysicsComponent(ent).slope) then
        onSlopeLeft = true
        SlopeNormal = normal1
        end
    end)
    if(onSlopeLeft or onSlopeRight) then
            Player.velocity.y=0
    end
    onSlopeLeft = false
    onSlopeRight = false
    Player.velocity.x = 0
    if(Input.isKeyDown(Keys["KEY_D"]) or Input.isKeyDown(Keys["KEY_RIGHT"])) then
        local handled = false
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ 23,GetPhysicsComponent(1).position.y - 25),XVector.new(1,0,0),14,function(normal1,point,ent)
                if(GetPhysicsComponent(ent).slope) then
                    onSlopeRight = true
                    SlopeNormal = normal1
                end
        end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- 23,GetPhysicsComponent(1).position.y - 25),XVector.new(-1,0,0),14,function(normal1,point,ent)
                if(GetPhysicsComponent(ent).slope) then
    
                    onSlopeLeft = true
                    SlopeNormal = normal1
                end
        end)

        if(onSlopeRight) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + 25 + GetClimbX(SlopeSpeed,Radians(45)),GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,function(normal2,point,ent)
                    local Direction = point - XVector.new(GetPhysicsComponent(1).position.x+25,GetPhysicsComponent(1).position.y-25)
                    if(SlopeNormal ~= normal2)then
                        Player.velocity = XVector.new(GetClimbX(SlopeSpeed,Radians(45)),GetClimbY(SlopeSpeed,Radians(45)))
                        --Player.velocity = Direction
                    else
                        Player.velocity = Direction
                    end

                    handled = true
            end)
        end

        if(onSlopeLeft) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - 25 + GetClimbX(SlopeSpeed,Radians(-45)),GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,function(normal2,point,ent)

                        local Direction = point - XVector.new(GetPhysicsComponent(1).position.x-25,GetPhysicsComponent(1).position.y-25)
                        if(SlopeNormal ~= normal2)then
                            Player.velocity = XVector.new(GetClimbX(SlopeSpeed,Radians(-45)),GetClimbY(SlopeSpeed,Radians(-45)))
                            Descending = true
                        else
                            Player.velocity = Direction
                            Descending = true
                        end
                        handled = true
            end)
        end


        if(not handled) then
            Player.velocity.x = 10
        end
    elseif(Input.isKeyDown(Keys["KEY_A"])or Input.isKeyDown(Keys["KEY_LEFT"])) then
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ 23,GetPhysicsComponent(1).position.y - 25),XVector.new(1,0,0),14,function(normal1,point,ent)
                if(GetPhysicsComponent(ent).slope) then
                    onSlopeRight = true
                    SlopeNormal = normal1
                end
        end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- 23,GetPhysicsComponent(1).position.y - 25),XVector.new(-1,0,0),14,function(normal1,point,ent)
                if(GetPhysicsComponent(ent).slope) then
    
                    onSlopeLeft = true
                    SlopeNormal = normal1
                end
        end)

        local handled = false

        if(onSlopeRight) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + 25 - GetClimbX(SlopeSpeed,45),GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,function(normal2,point,ent)
                    local Direction = point - XVector.new(GetPhysicsComponent(1).position.x+25,GetPhysicsComponent(1).position.y-25)
                    if(SlopeNormal ~= normal2)then
                        Descending = true
                        Player.velocity = XVector.new(GetClimbX(-SlopeSpeed,Radians(45)),GetClimbY(-SlopeSpeed,Radians(45)))
                        --Player.velocity = Direction
                    else
                        Descending = true
                        Player.velocity = Direction
                    end

                    handled = true
            end)
        end

        if(onSlopeLeft) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - 25 - GetClimbX(SlopeSpeed,Radians(-45)) ,GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,function(normal2,point,ent)

                        local Direction = point - XVector.new(GetPhysicsComponent(1).position.x-25,GetPhysicsComponent(1).position.y-25)
                        if(SlopeNormal ~= normal2)then
                            Player.velocity = XVector.new(GetClimbX(-SlopeSpeed,Radians(-45)),GetClimbY(-SlopeSpeed,Radians(-45)))

                        else
                            Player.velocity = Direction
                        end
                        handled = true
            end)
        end

        if(not handled) then
            Player.velocity.x = -10
        end
    end
    if(Input.isKeyDown(Keys["KEY_SPACE"])) then
        PhysicsSystem.rayCast(GetPhysicsComponent(1).position,XVector.new(0,-1,0),26,function(normal,point,ent)Player.velocity.y =  40 end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+25,GetPhysicsComponent(1).position.y-25),XVector.new(0,-1,0),1,function(normal,point,ent)  Player.velocity.y =  40 end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x-25,GetPhysicsComponent(1).position.y-25),XVector.new(0,-1,0),1,function(normal,point,ent)   Player.velocity.y = 40 end)

    end

end
returnTable["LateFixedUpdate"] = function (dt)


    Player.velocity.y = Player.velocity.y - 1.2

    handleMovment()
end
return returnTable
