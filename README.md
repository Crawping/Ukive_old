# Ukive
һ���򵥵�UI��ܣ���̬����ģ�� Android ϵͳ�Ļ��ơ�
���� Google �� Material Design ��Ʊ�׼��
ʹ�� DirectX 11.1 ��Ⱦ��

![Demo](https://github.com/ucclkp/Ukive/tree/master/Screenshot/demo.png)

DirectXMath �ļ�����Ϊ΢��Ŀ�Դ��ѧ�⡣����֧��ʹ�� SSE ָ���

��Ϊ Windows Ӧ�ó�����Ŀ��ʹ�� Visual Studio Community 2017 ��д��
������л���Ϊ Windows 7 SP1 with DirectX 11.1��
������ʹ���� C++ ���±�׼�е����ԣ��� nullptr��lambda ���ʽ��
������ʹ�� C++ ��׼�⡣

������� WinMain λ�� Main.cpp �ļ��С�
������ѭ��λ�� UApplication.cpp �У�����Ҳ�ڴ˶��塣
�� Windows ����Ϣ�����⣬��������ά��һ��Ϣ���У�����Ϣ���д����� UCycler ��������Ϣ���μ� UMessageLooper ��� UCycler �ࡣ
UCycler ��Ӧ�� Android �е� Handler��

UDeviceManager ���ж��� DirectX �豸��
DirectX �Ľ����������� URenderer ���У�URenderer ���������һϵ�����豸��ص���Դ�ķ�����Ҳ�������Ʒ�����ÿһ�����嶼ӵ��һ�� URenderer ��ʵ����

UWindow ���Ǵ���Ļ��࣬����һ�����͵� Windows ���塣������ LodWindow Ϊ LOD ��صĴ��塣
UWidget ���ǿؼ��Ļ��ࡣ�ؼ�����UI�����û����о��彻����Ԫ�أ��簴ť���ı���ȡ�
UWidgetGroup ��̳��� UWidget �࣬���п���� UWidget �� UWidgetGroup���γ����νṹ��

Ŀǰ������Ԥ����ؼ���
* UTextView: �ı���֧�ֳ�����̡�֧�ֱ༭�Ͳ鿴ģʽ���� TSF ֧�����뷨���룬 Windows10 �Դ����뷨����ͨ�����ݲ�֧�� RTL��
* UButton: ��ť��ʹ�� Ripple ��Ч��
* UImageView: ͼƬ�򣬿���ʾͼ��
* USeekBar: �϶�����
* UDirect3DView: 3D��ͼ�ؼ�����λ�ü���С������3D��ͼ�ڴ����ϵ�λ�úʹ�С��ÿ�����������ֻ����һ����
* USpinnerView: ����ѡ��ؼ���δ��ɣ���
* UBaseLayout: ÿ����������ֻ��һ����Ϊ�ؼ����ĸ���
* UFrameLayout: ��򵥵Ĳ��֡����ܶ�Ӧ�� Android �� FrameLayout��
* ULinearLayout: �������в��֡����ܶ�Ӧ�� Android �� LinearLayout��
* UListView: �б�δ��ɣ���
* UScrollView: �������֡����ܶ�Ӧ�� Android �� ScrollView��
* RestrainLayout: ���ڹ�ϵ�Ĳ��֣���Ϊ���ӡ����ܶ�Ӧ�� Android �� ConstraintLayout��

## Licenses
* GPL v3.0
* DirectXMath: MIT