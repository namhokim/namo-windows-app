using System.Runtime.InteropServices;   // for DllImport

namespace Namo.Time
{
    class StopWatch
    {
        // 마이크로초를 밀리초로 바꾸기 위한 단위입니다
        private const int MilliUnit = 1000;

        // Win32 네이티브 함수입니다.
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool QueryPerformanceFrequency([In] ref long lpFrequency);
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool QueryPerformanceCounter([In] ref long lpFrequency);

        // 맴버변수입니다.
        private long m_frequency;
        private long m_t1;
        private long m_t2;
        public bool m_isSupport;    // 고해상도 타이머를 지원하는지
        

        /// <summary>
        /// 기본 생성자 입니다.
        /// </summary>
        public StopWatch()
        {
            m_isSupport = QueryPerformanceFrequency(ref m_frequency);
            m_t2 = 0;
            QueryPerformanceCounter(ref m_t1);
        }

        /// <summary>
        /// StopWatch의 시작합니다.
        /// </summary>
        /// <returns>
        /// 성공여부를 반환합니다.
        /// </returns>
        public bool Start()
        {
            return QueryPerformanceCounter(ref m_t1);
        }

        /// <summary>
        /// StopWatch를 중지합니다.
        /// </summary>
        /// <returns>
        /// 성공여부를 반환합니다.
        /// </returns>
        public bool Stop()
        {
            return QueryPerformanceCounter(ref m_t2);
        }

        /// <summary>
        /// Start()~Stop() 사이간 시간을 반환합니다.
        /// </summary>
        /// <returns>
        /// ㎳ 단위 double형태로 반환합니다.
        /// </returns>
        public double LapTime()
        {
            return (((double)(m_t2 - m_t1) / m_frequency) * MilliUnit);
        }
    }
}
