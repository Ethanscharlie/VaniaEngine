local speed = 300
local cooldownTime = 0.3

local function setup(entity)
	setPropNum(entity, "cooldown", 0)
end

local function update(entity, deltaTime)
	if getButtonHeld("a") then
		entity.angle = entity.angle - speed * deltaTime
	end
	if getButtonHeld("d") then
		entity.angle = entity.angle + speed * deltaTime
	end
	if getButtonHeld("w") then
		moveForward(entity, speed * deltaTime)
	end

	print(getPropNum(entity, "cooldown"))
	print(os.clock())
	local onCooldown = os.clock() < cooldownTime + getPropNum(entity, "cooldown")
	if getButtonHeld("f") and not onCooldown then
		bullet = summon("Bullet", entity.x, entity.y)
		bullet.angle = entity.angle
		setPropNum(entity, "cooldown", os.clock())
	end
end

return {
	setup = setup,
	update = update,
}
