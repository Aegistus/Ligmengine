struct Uniforms {
	projection: mat4x4f,
};

@group(0) @binding(0) var<uniform> uniforms: Uniforms;
@group(0) @binding(1) var texSampler: sampler;
@group(0) @binding(2) var texData: texture_2d<f32>;

struct VertexInput {
	@location(0) position: vec2f,
	@location(1) texcoords: vec2f,
	@location(2) translation: vec3f,
	@location(3) scale: vec2f,
    @location(4) rotation: vec2f
};

struct VertexOutput {
	@builtin(position) position: vec4f,
	@location(0) texcoords: vec2f,
};

@vertex
fn vertex_shader_main( in: VertexInput ) -> VertexOutput {
	var out: VertexOutput;
	var rotatedPos: vec2f = vec2f
	(
		in.position.x * in.rotation.y - in.position.y * in.rotation.x,
		in.position.x * in.rotation.x + in.position.y * in.rotation.y
	);
	rotatedPos *= in.scale;
	out.position = uniforms.projection * vec4f( vec3f( rotatedPos, 0.0 ) + in.translation, 1.0 );
	out.texcoords = in.texcoords;
	return out;
}

@fragment
fn fragment_shader_main( in: VertexOutput ) -> @location(0) vec4f {
	let color = textureSample( texData, texSampler, in.texcoords ).rgba;
	return color;
}