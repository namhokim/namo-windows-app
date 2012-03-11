class SomeClass 
{
	// 사전 정의된 코드값(C에서는 이것을 헤더로 만들어서 리턴값 판단)
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
