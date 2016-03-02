<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:output indent="yes" />
	<xsl:variable name="numVehicles" select="metadata/numVehicles" />

	<xsl:template name="vehicle">
		<xsl:param name="i" />
		<xsl:if test="$i &lt; $numVehicles">
			<include>
				<uri>model://gcVehicle</uri>
				<name>vehicle<xsl:value-of select="$i" /></name>
			</include>
			<include>
				<uri>model://wheel</uri>
				<name>vehicle<xsl:value-of select="$i" />::wheel0</name>
			</include>
			<include>
				<uri>model://wheel</uri>
				<name>vehicle<xsl:value-of select="$i" />::wheel1</name>
			</include>
			<include>
				<uri>model://wheel</uri>
				<name>vehicle<xsl:value-of select="$i" />::wheel2</name>
			</include>
			<include>
				<uri>model://wheel</uri>
				<name>vehicle<xsl:value-of select="$i" />::wheel3</name>
			</include>
			<xsl:call-template name="vehicle">
				<xsl:with-param name="i" select="$i+1" />
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template match="/">
		<sdf version="1.5">
			<world name="GcWorld">
				<scene>
					<sky>
						<clouds>
							<speed>5.0</speed>
							<direction>1 0 0 </direction>
						</clouds>
					</sky>
				</scene>

				<light type="directional" name="sun">
					<pose>100 0 100 0 -0.785 0</pose>
					<diffuse>1 1 1 1</diffuse>
					<specular>.1 .1 .1 1</specular>
					<attenuation>
						<range>5000</range>
						<linear>0.2</linear>
						<constant>0.8</constant>
						<quadratic>0.01</quadratic>
					</attenuation>
					<cast_shadows>true</cast_shadows>
				</light>

				<model name="my_ground_plane">
					<pose>0 0 0 0 0 0</pose>
					<static>true</static>
					<link name="link">
						<frame name="ground" />
						<pose>0 0 0 0 0 0</pose>
						<visual name="visual">
							<cast_shadows>true</cast_shadows>
							<geometry>
								<plane>
									<normal>0 0 1</normal>
									<size>500 500</size>
								</plane>
							</geometry>
							<material>
								<script>
									<uri>file://../data/gazono.material</uri>
									<name>Gazebo/Ground01</name>
								</script>

							</material>
						</visual>
						<!--
						<sensor name="topcamera" type="camera">
							<pose>-70 0 30 0 .62 0</pose>
							<camera name="secCam">
								<horizontal_fov>1.57</horizontal_fov>
								<image>
									<format>B8G8R8</format>
									<width>2560</width>
									<height>1440</height>
								</image>
								<clip>
									<near>0.1</near>
									<far>500</far>
								</clip>
								<noise>
									<type>gaussian</type>
									<mean>0.0</mean>
									<stddev>0.000</stddev>
								</noise>
								<save enabled="true">
									<path>Captures/SecCam/2016_03_02_2k</path>
								</save>
							</camera>
							<always_on>1</always_on>
							<update_rate>60</update_rate>
							<visualize>true</visualize>
						</sensor>
					-->
					<!--
						<sensor name="sidecamera" type="camera">
							<pose>-55 0 2 0 .2 .6</pose>
							<camera name="sideCam">
								<horizontal_fov>1.57</horizontal_fov>
								<image>
									<format>B8G8R8</format>
									<width>2560</width>
									<height>1440</height>
								</image>
								<clip>
									<near>0.1</near>
									<far>500</far>
								</clip>
								<noise>
									<type>gaussian</type>
									<mean>0.0</mean>
									<stddev>0.000</stddev>
								</noise>
								<save enabled="true">
									<path>Captures/SideCam/2016_03_02_2k</path>
								</save>
							</camera>
							<always_on>1</always_on>
							<update_rate>60</update_rate>
							<visualize>true</visualize>
						</sensor>
					-->
					</link>
				</model>

				<xsl:call-template name="vehicle">
					<xsl:with-param name="i" select="0" />
				</xsl:call-template>

				<road name="circular_road">
					<width>7.5</width>
					<point>36.605637 0.000000 0.05</point>
					<point>36.316991 4.587903 0.05</point>
					<point>35.455604 9.103452 0.05</point>
					<point>34.035061 13.475434 0.05</point>
					<point>32.077764 17.634900 0.05</point>
					<point>29.614582 21.516254 0.05</point>
					<point>26.684361 25.058283 0.05</point>
					<point>23.333311 28.205128 0.05</point>
					<point>19.614281 30.907162 0.05</point>
					<point>15.585922 33.121771 0.05</point>
					<point>11.311764 34.814030 0.05</point>
					<point>6.859212 35.957251 0.05</point>
					<point>2.298487 36.533404 0.05</point>
					<point>-2.298487 36.533404 0.05</point>
					<point>-6.859212 35.957251 0.05</point>
					<point>-11.311764 34.814030 0.05</point>
					<point>-15.585922 33.121771 0.05</point>
					<point>-19.614281 30.907162 0.05</point>
					<point>-23.333311 28.205128 0.05</point>
					<point>-26.684361 25.058283 0.05</point>
					<point>-29.614582 21.516254 0.05</point>
					<point>-32.077764 17.634900 0.05</point>
					<point>-34.035061 13.475434 0.05</point>
					<point>-35.455604 9.103452 0.05</point>
					<point>-36.316991 4.587903 0.05</point>
					<point>-36.605637 -0.000000 0.05</point>
					<point>-36.316991 -4.587903 0.05</point>
					<point>-35.455604 -9.103452 0.05</point>
					<point>-34.035061 -13.475434 0.05</point>
					<point>-32.077764 -17.634900 0.05</point>
					<point>-29.614582 -21.516254 0.05</point>
					<point>-26.684361 -25.058283 0.05</point>
					<point>-23.333311 -28.205128 0.05</point>
					<point>-19.614281 -30.907162 0.05</point>
					<point>-15.585922 -33.121771 0.05</point>
					<point>-11.311764 -34.814030 0.05</point>
					<point>-6.859212 -35.957251 0.05</point>
					<point>-2.298487 -36.533404 0.05</point>
					<point>2.298487 -36.533404 0.05</point>
					<point>6.859212 -35.957251 0.05</point>
					<point>11.311764 -34.814030 0.05</point>
					<point>15.585922 -33.121771 0.05</point>
					<point>19.614281 -30.907162 0.05</point>
					<point>23.333311 -28.205128 0.05</point>
					<point>26.684361 -25.058283 0.05</point>
					<point>29.614582 -21.516254 0.05</point>
					<point>32.077764 -17.634900 0.05</point>
					<point>34.035061 -13.475434 0.05</point>
					<point>35.455604 -9.103452 0.05</point>
					<point>36.316991 -4.587903 0.05</point>
					<point>36.605637 0 0.05</point>
					<material>
						<script>
							<uri>file://../data/gazono.material</uri>
							<name>Gazebo/Line</name>
						</script>
					</material>
				</road>



				<plugin name="chrono_gazebo" filename="libchrono_gazebo.so" />
			</world>
		</sdf>

	</xsl:template>

</xsl:stylesheet>
