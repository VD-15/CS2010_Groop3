package engine.graphics;

/**
 * Represents a 4-channel color with normalized floating points
 *
 */
public class Color implements Cloneable
{
	/**
	 * The red component of this color
	 */
	public float r;
	
	/**
	 * The green component of this color
	 */
	public float g;
	
	/**
	 * The blue component of this color
	 */
	public float b;
	
	/**
	 * The alpha (opacity) component of this color
	 */
	public float a;
	
	/**
	 * Constructs a white color
	 */
	public Color()
	{
		this.r = 1f;
		this.g = 1f;
		this.b = 1f;
		this.a = 1f;
	}
	
	/**
	 * Constructs a color with the given color components and an alpha of one
	 * @param red The red component of the color within range 0..1.
	 * @param green The green component of the color within range 0..1
	 * @param blue The blue component of the color within range 0..1
	 */
	public Color(float red, float green, float blue)
	{
		this.r = red;
		this.g = green;
		this.b = blue;
		this.a = 1f;
	}
	
	/**
	 * Constructs a color with the given color components
	 * @param red The red component of the color within range 0..1.
	 * @param green The green component of the color within range 0..1.
	 * @param blue The blue component of the color within range 0..1.
	 * @param alpha The opacity of the color within range 0..1.
	 */
	public Color(float red, float green, float blue, float alpha)
	{
		this.r = red;
		this.g = green;
		this.b = blue;
		this.a = alpha;
	}
	
	/**
	 * Clones a color object
	 * @param c The color to be cloned
	 */
	public Color(Color c)
	{
		this.a = c.a;
		this.g = c.g;
		this.b = c.b;
		this.a = c.a;
	}
	
	/**
	 * Multiplies this color
	 * @param c The color to multiply by.
	 * @see {@link #multiply(float)}
	 */
	public void multiply(Color c)
	{
		this.r *= c.r;
		this.g *= c.g;
		this.b *= c.b;
		this.a *= c.a;
	}

	/**
	 * Multiplies this color
	 * @param s The factor to multiply this color by.
	 * @see {@link #multiply(Color)}
	 * @see {@link #multiplyAlpha(float)}
	 */
	public void multiply(float s)
	{
		this.r *= s;
		this.g *= s;
		this.b *= s;
		this.a *= s;
	}
	
	/**
	 * Adds to this color
	 * @param c The color to add to this color.
	 */
	public void add(Color c)
	{
		this.r += c.r;
		this.g += c.g;
		this.b += c.b;
		this.a += c.a;
	}
	
	@Override
	public boolean equals(Object o)
	{
		Color c = (Color)o;
		if (c.r != this.r) return false;
		if (c.g != this.g) return false;
		if (c.b != this.b) return false;
		if (c.a != this.a) return false;
		return true;
	}
	
	@Override
	public Color clone()
	{
		return new Color(this);
	}
	
	@Override
	public String toString()
	{
		return this.getClass().getName() + ' ' + Float.toString(this.r) + ' ' + Float.toString(this.g) + ' ' + Float.toString(this.b) + ' ' + Float.toString(this.a);
	}
}
