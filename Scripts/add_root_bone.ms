root = BoneSys.createBone [0, 0, 0] [1, 0, 0] [0, 0, 1]
root.name = "root"

hips = getNodeByName "mixamorig:Hips"
select hips

for key in hips.position.controller.keys do
(
	sliderTime = key.Time
	root_position = hips.position
	root_position.z = 0
	hips_position = hips.position
	hips_position.x = 0
	hips_position.y = 0
	animate on
	(
			at time key.Time
				root.position = root_position
				hips.position = hips_position
	)
)

sliderTime = 0
hips.parent = root