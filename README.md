# StickItToEm
A remake of the game I made for Uni coursework, made using OpenGL

TODO:
	TerrainManager:
		Step1:
			Take in size of arena (number of squares (x,y))
				Create array for scalar data, lines for collision, and buffers for rendering terrain triangles/lines
			Take in scalar data to generate arena
				Take in pointer to array, copy data to array in terrainManager
			Generate terrain using marching square algorithm
				Create function to generate marchingSquares
				Generate triangles and lines
					Fill buffers for rendering triangles/lines

			Render triangles and lines to screen
				Marching squares takes up full screen irregardless of screen size
					Use screen size and grid size to transform vertex positions
		Step2:
			Have function that can modify values within the scalar field at co-ordinate position
				Function converts co-ordinates into correct index in array
				Function clamps values in scalar field between -1 and +1
			Have function that can modify values in a circular area (simulate explosion)
				Amount removed decreases further from centre
			Functions regenerate affected squares
				Generates new triangles/lines
				Finds index of changed vertices within buffers
					Values on different columns will not be contigous, several changes of data will be needed
				Updates lines stored in the same way
	Player:

	Collision:

	Weapons:

