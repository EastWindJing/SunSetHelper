#ifndef __SUNPOSITION_H__
#define __SUNPOSITION_H__



#ifndef M_PI
  #define M_PI 3.14159265358979323846264338327950288
#endif

/**
 * class SunSet
 * 
 * This class controls all aspects of the operations. The math is done in private
 * functions, and the public API's allow for returning a sunrise/sunset value for the
 * given coordinates and timezone.
 * 这个类控制着所有操作的各个方面。数学运算在私有函数中进行，公共API允许根据给定的经纬度和时区返回日出/日落值。
 * 
 * The resulting calculations are relative to midnight of the day you set in the
 * setCurrentDate() function. It does not return a time_t value for delta from the
 * current epoch as that would not make sense as the sunrise/sunset can be calculated
 * thousands of years in the past. The library acts on a day timeframe, and doesn't
 * try to assume anything about any other unit of measurement other than the current
 * set day.
 * 最终的计算结果是基于您在 `setCurrentDate()` 函数中设置的日期的午夜时间。它不会返回从当前纪元开始的毫秒数，
 * 因为从当前纪元开始的毫秒数与日出/日落时间的计算无关，而且这种计算可以追溯到几千年前。该库基于一天的时间框架进行操作，
 * 不会假定任何其他与当前设置的日期不同的时间单位。
 * 
 * You can instantiate this class a few different ways, depending on your needs. It's possible
 * to set your location one time and forget about doing that again if you don't plan to move.
 * Then you only need to change the date and timezone to get correct data. Or, you can
 * simply create an object with no location or time data and then do that later. This is
 * a good mechanism for the setup()/loop() construct.
 * 你可以用几种不同的方式实例化这个类，这取决于你的需求。如果你不打算移动，可以一次性设置你的位置，然后不再考虑此事。
 * 这样，你只需要更改日期和时区以获取正确的数据。或者，你可以创建一个没有任何位置或时间数据的对象，然后稍后再进行设置。
 * 这是适用于setup()/loop()结构的好机制。
 * 
 * The most important thing to remember is to make sure the library knows the exact date and
 * timezone for the sunrise/sunset you are trying to calculate. Not doing so means you are going
 * to have very odd results. It's reasonably easy to know when you've forgotten one or the other
 * by looking at the time the sun would rise and noticing that it is X hours earlier or later.
 * That is, if you get a return of 128 minutes (2:08 AM) past midnight when the sun should rise 
 * at 308 (6:08 AM), then you probably forgot to set your EST timezone.
 * 最重要的是要确保库知道你试图计算的日出/日落的确切日期和时区。否则，你将得到非常奇怪的结果。
 * 通过查看太阳升起的时间并注意到它比预期早了X小时或晚了X小时，你可以相当容易地知道是否忘记了其中一个。
 * 也就是说，如果你在午夜之后得到128分钟（2:08 AM）的返回值，而太阳应该在308分钟（6:08 AM）升起，那么你很可能忘记设置EST时区（美国东部标准时间）。
 * 
 * The library also has no idea about daylight savings time. If your timezone changes during the
 * year to account for savings time, you must update your timezone accordingly.
 * 这个库不清楚夏令时。如果您的时区在一年中因夏令时而发生变化，您必须相应地更新您的时区。
 * （Daylight Savings Time 夏令时：在天亮早的夏季人为将时间调快一小时，可以使人早起早睡，减少照明量，从而节约照明用电的制度。
 *   在这一制度实行期间所采用的统一时间称为“夏令时间”）
 */


class SunSet {
public:
    SunSet();
    SunSet(double, double, int);
    SunSet(double, double, double);
    ~SunSet();

    static constexpr double SUNSET_OFFICIAL = 90.833;       /**< Standard sun angle for sunset */
    static constexpr double SUNSET_CIVIL = 96.0;            /**< Civil sun angle for sunset */
    static constexpr double SUNSET_NAUTICAL = 102.0;        /**< Nautical sun angle for sunset */
    static constexpr double SUNSET_ASTONOMICAL = 108.0;     /**< Astronomical sun angle for sunset */
    
    void setPosition(double, double, int);      //设置当前位置（纬度、经度、时区）
    void setPosition(double, double, double);
    void setTZOffset(int);                      //设置当前位置的时区（西为负，东为正）
    void setTZOffset(double);
    double setCurrentDate(int, int, int);       //输入当前公历日期，返回朱利安日期
    double calcCivilSunrise() const;            //计算民用日出
    double calcCivilSunset() const;             //计算民用日落（太阳刚落至地平线以下6°）,返回午夜过后的小数分钟为单位返回民用日落
    double calcNauticalSunrise() const;         //计算航海日出
    double calcNauticalSunset() const;          //计算航海日落（太阳刚落至地平线以下12°）
    double calcAstronomicalSunrise() const;     //计算天文日出
    double calcAstronomicalSunset() const;      //计算天文日落（太阳刚落至地平线以下18°）
    double calcCustomSunrise(double) const;     //计算定制日出
    double calcCustomSunset(double) const;      //计算定制日落（参数是太阳的天顶角，正午为0°，到达地平线为90°）
    [[deprecated("UTC specific calls may not be supported in the future")]] double calcSunriseUTC();
    [[deprecated("UTC specific calls may not be supported in the future")]] double calcSunsetUTC();
    double calcSunrise() const;
    double calcSunset() const;
    //[[deprecated]] 是 C++11 引入的一个属性，用于标记某个函数、类或变量已被弃用（不推荐使用）。
    //通过 [[deprecated]] 标记的函数，编译器通常会生成警告（而非错误），提醒开发者该函数不再推荐使用。

    int moonPhase(int) const;       //月相，返回值为0到29，其中0和29为新月，14为满月。
    int moonPhase() const;
    
private:
    double degToRad(double) const;
    double radToDeg(double) const;
    double calcMeanObliquityOfEcliptic(double) const;
    double calcGeomMeanLongSun(double) const;
    double calcObliquityCorrection(double) const;
    double calcEccentricityEarthOrbit(double) const;
    double calcGeomMeanAnomalySun(double) const;
    double calcEquationOfTime(double) const;
    double calcTimeJulianCent(double) const;
    double calcSunTrueLong(double) const;
    double calcSunApparentLong(double) const;
    double calcSunDeclination(double) const;
    double calcHourAngleSunrise(double, double, double) const;
    double calcHourAngleSunset(double, double, double) const;
    double calcJD(int,int,int) const;
    double calcJDFromJulianCent(double) const;
    double calcSunEqOfCenter(double) const;
    double calcAbsSunrise(double) const;
    double calcAbsSunset(double) const;

    double m_latitude;
    double m_longitude;
    double m_julianDate;
    double m_tzOffset;
    int m_year;
    int m_month;
    int m_day;
};

#endif
