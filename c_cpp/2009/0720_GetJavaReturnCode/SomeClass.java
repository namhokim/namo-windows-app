class SomeClass 
{
	// ���� ���ǵ� �ڵ尪(C������ �̰��� ����� ���� ���ϰ� �Ǵ�)
	static final int NormalExit = 0;
	static final int AbnormalExit = 1010;
	public static void main(String[] args) 
	{
		int argc = args.length;
		System.out.println("argument numbers : " + argc);
		if(args.length == 0) {
			System.out.println("Excute some good jobs!");
			System.exit(NormalExit);
		}
		else {
			System.out.println("Excute some bad jobs!");
			System.exit(AbnormalExit);
		}
	}
}
