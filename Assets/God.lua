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
local onGround = false
local PlayerScale = XVector.new(32,32)
function math.sign(x)
    if x<0 then
        return -1
    elseif x>0 then
        return 1
    else
        return 0
    end
end
function math.round(x)
  return x>=0 and math.floor(x+0.5) or math.ceil(x-0.5)
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
local handleMovment = function(dt)
 
    local acvel = Player.velocity * dt
 if(Descending) then

        if(acvel.y~=0) then

            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(0,acvel.y),acvel.x,0,false)

        end

        if(acvel.x~=0) then
            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(acvel.x,0),0,acvel.y,false)
        end

    else
        if(acvel.x~=0) then
            PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(acvel.x,0),0,acvel.y,false)
        end
        if(acvel.y~=0) then
            if(PhysicsSystem.MovePlayer(GetPhysicsComponent(1).position,XVector.new(0,acvel.y),acvel.x,0,false)) then
            Player.velocity.y = 0
            end
        end
    end

end
returnTable["Init"] = function ()

end
returnTable["Update"] = function(dter)
    Descending = false
    --Player.velocity.y = 0

    onGround = false
    
    onSlopeRight = false
    onSlopeLeft = false
    local SlopeSpeed = 20
    local buildupspeed = 20
    local dt = 0.01666666667
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ PlayerScale.x - 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(1,0,0),4,(1 << 6),function(normal1,point,distance,ent)
        if(GetPhysicsComponent(ent).slope) then
        onSlopeRight = true
        SlopeNormal = normal1
    end
    end)
    PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- PlayerScale.x + 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(-1,0,0),4,(1 << 6),function(normal1,point,distance,ent)
        if(GetPhysicsComponent(ent).slope) then

        onSlopeLeft = true
        SlopeNormal = normal1
        end
    end)
    SlopeNormal = XVector.new(0,0)
    if(onSlopeLeft or onSlopeRight) then
            Player.velocity.y=0
    else
    end
    onSlopeRight = false
    onSlopeLeft = false
    Player.velocity.x = 0
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x,GetPhysicsComponent(1).position.y - PlayerScale.y + 1),XVector.new(0,-1,0),2,function(normal,point,distance,ent)if(distance > 1) then   onGround = true end end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+PlayerScale.x,GetPhysicsComponent(1).position.y - PlayerScale.y + 1),XVector.new(0,-1,0),2,function(normal,point,distance,ent) if(distance > 1) then  onGround = true end end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x-PlayerScale.x,GetPhysicsComponent(1).position.y - PlayerScale.y + 1),XVector.new(0,-1,0),2,function(normal,point,distance,ent) if(distance > 1) then   onGround = true end end)

    if(Input.isKeyDown(Keys["KEY_D"]) or Input.isKeyDown(Keys["KEY_RIGHT"])) then
        local handled = false
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ PlayerScale.x - 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(1,0,0),14,(1 << 6),function(normal1,point,distance,ent)
                if(GetPhysicsComponent(ent).slope) then
                    onSlopeRight = true
                    SlopeNormal = normal1
                end
        end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- PlayerScale.x + 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(-1,0,0),14,(1 << 6),function(normal1,point,distance,ent)
                if(GetPhysicsComponent(ent).slope) then

                    onSlopeLeft = true
                    SlopeNormal = normal1
                end
        end)



        if(onSlopeRight and onGround) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + PlayerScale.x + GetClimbX(SlopeSpeed ,Radians(45)),GetPhysicsComponent(1).position.y + PlayerScale.y),XVector.new(0,-1,0),100000,(1 << 6),function(normal4,poin7t,disasdtance,enbt) 

            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + PlayerScale.x + GetClimbX(SlopeSpeed ,Radians(45)),GetPhysicsComponent(1).position.y + PlayerScale.y),XVector.new(0,-1,0),100000,function(normal2,point,distance,ent)
                    print("RIGHT SLOPE")
                   onGround = true
                   local Direction = point - XVector.new(GetPhysicsComponent(1).position.x+PlayerScale.x,GetPhysicsComponent(1).position.y-PlayerScale.y)

                   local Distancefromedge = -1
                   PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x  +PlayerScale.x,GetPhysicsComponent(1).position.y+PlayerScale.y),XVector.new(1,0,0), GetClimbX(SlopeSpeed,Radians(45) ),function(normal2,points,distancer,ent)   if(GetPhysicsComponent(ent).onSlope) then Distancefromedge = distancer  end end)

                   if(SlopeNormal ~= normal2)then
                        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + PlayerScale.x + Distancefromedge - 0.1 ,GetPhysicsComponent(1).position.y + 100),XVector.new(0,-1,0),100000,function(normal2er,pointer,distance,ent)   Direction = pointer - XVector.new(GetPhysicsComponent(1).position.x+PlayerScale.x,GetPhysicsComponent(1).position.y - PlayerScale.y)   end)
                            Descending = true
                         Player.velocity = XVector.new(GetClimbX(buildupspeed,Radians(45)),GetClimbY(buildupspeed,Radians(45))) / dt
                         if(Distancefromedge ~= -1) then
                             Player.velocity = Direction / dt
                         end

                else
                Descending = true
                Player.velocity = Direction / dt
                    end
                        handled = true


            end)
        end)
        end

        if(onSlopeLeft) then
           PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x + GetClimbX(SlopeSpeed,Radians(-45)),GetPhysicsComponent(1).position.y + PlayerScale.y),XVector.new(0,-1,0),100000,(1 << 6),function(normal4,point3,distanceb,entdf)
           
           PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x + GetClimbX(SlopeSpeed,Radians(-45)),GetPhysicsComponent(1).position.y + PlayerScale.y),XVector.new(0,-1,0),100000,function(normal2,point,distance,ent)
                        onGround = true
                        local Direction = point - XVector.new(GetPhysicsComponent(1).position.x-PlayerScale.x,GetPhysicsComponent(1).position.y-PlayerScale.y)
                        if(SlopeNormal ~= normal2)then
                            Player.velocity = XVector.new(GetClimbX(buildupspeed,Radians(-45)),GetClimbY(buildupspeed,Radians(-45))) / dt
                        else 
                            Player.velocity = Direction / dt
             
                        end
                        handled = true

            end)
       end)
        end


        if(not handled) then
            Player.velocity.x = 10 / dt
        end
    elseif(Input.isKeyDown(Keys["KEY_A"])or Input.isKeyDown(Keys["KEY_LEFT"])) then
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x+ PlayerScale.x - 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(1,0,0),14,(1 << 6),function(normal1,point,distance,ent)
                if(GetPhysicsComponent(ent).slope) then
                    onSlopeRight = true
                    SlopeNormal = normal1
                end
        end)
        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x- PlayerScale.x + 2,GetPhysicsComponent(1).position.y - PlayerScale.y),XVector.new(-1,0,0),14,(1 << 6),function(normal1,point,distance,ent)
                if(GetPhysicsComponent(ent).slope) then

                    onSlopeLeft = true
                    SlopeNormal = normal1
                end
        end)
        local handled = false

        if(onSlopeRight) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x + PlayerScale.x - GetClimbX(SlopeSpeed,45),GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,(1 << 6),function(normal2,point,distance,ent)
                    onGround = true
                    local Direction = point - XVector.new(GetPhysicsComponent(1).position.x+PlayerScale.x,GetPhysicsComponent(1).position.y-PlayerScale.y)
                    if(SlopeNormal ~= normal2)then
                      Player.velocity = XVector.new(GetClimbX(-buildupspeed,Radians(45)),GetClimbY(-buildupspeed,Radians(45))) / dt
                        --Player.velocity = Direction
                    else
                        Player.velocity = Direction / dt
                         end

                    handled = true
            end)
        end

        if(onSlopeLeft) then
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x - GetClimbX(SlopeSpeed,Radians(-45)) ,GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,(1 << 6),function(normal4,point,distance,ent)
            PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x - GetClimbX(SlopeSpeed,Radians(-45)) ,GetPhysicsComponent(1).position.y + 20),XVector.new(0,-1,0),100000,function(normal2,point,distance,ent)
            
                        onGround = true
                        local Direction = point - XVector.new(GetPhysicsComponent(1).position.x-PlayerScale.x,GetPhysicsComponent(1).position.y-PlayerScale.y)
                         local Distancefromedge = -1
                         PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x,GetPhysicsComponent(1).position.y),XVector.new(-1,0,0), GetClimbX(SlopeSpeed,Radians(45) ),function(normal2,points,distancer,ent)   if(GetPhysicsComponent(ent).onSlope) then Distancefromedge = distancer   end end)
                      if(SlopeNormal ~= normal2)then
                        Descending = true
                        PhysicsSystem.rayCast(XVector.new(GetPhysicsComponent(1).position.x - PlayerScale.x - Distancefromedge + 0.1 ,GetPhysicsComponent(1).position.y + 100),XVector.new(0,-1,0),100000,function(normal2er,pointer,distancers,ent)   Direction = pointer - XVector.new(GetPhysicsComponent(1).position.x-PlayerScale.x,GetPhysicsComponent(1).position.y - PlayerScale.y)   end)
                         Player.velocity = XVector.new(GetClimbX(-buildupspeed,Radians(-45)),GetClimbY(-buildupspeed,Radians(-45))) / dt
                         if(Distancefromedge ~= -1) then
                          Player.velocity = Direction / dt
                         end

                        else
                            Player.velocity = Direction / dt
                                       Descending = true
end
                        handled = true
            end)
        end)

        end

        if(not handled) then
            Player.velocity.x = -10 / dt
        end
    end

    if(Input.isKeyDown(Keys["KEY_SPACE"]) and onGround) then
        Player.velocity.y = (40 / dt)
    
    end
if(not onGround )  then
    print("OAH WOW")
        if(Player.velocity.y > 0 ) then
    Player.velocity.y  = Player.velocity.y - (120.2 / dt) * dter 

    else
    Player.velocity.y = Player.velocity.y - (200.6 / dt) * dter


    end
end

    handleMovment(1/60)


  
end

local JumpRayCast = function(normal,point,distance,ent)


end
returnTable["FixedUpdate"] = function (dt)

end
returnTable["LateFixedUpdate"] = function (dt)
    --Camera.SetPosition(Easings.linear(0.1,Camera.GetPosition(),XVector.new(GetTransformComponent(1).position.x *-1 +500  ,Camera.GetPosition().y)- Camera.GetPosition() ,1))
    --Camera.SetPosition(XVector.new(math.round(Camera.GetPosition().x),math.round(Camera.GetPosition().y)))
end
return returnTable
