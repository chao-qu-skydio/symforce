import numpy


class LieGroupOps(object):
    """
    Python LieGroupOps implementatino for <class 'symforce.geo.pose2.Pose2'>.
    """

    @staticmethod
    def from_tangent(vec, epsilon):

        # Input arrays

        # Intermediate terms
        _tmp0 = numpy.cos(vec[2])
        _tmp1 = numpy.sin(vec[2])
        _tmp2 = 1/(epsilon + vec[2])
        _tmp3 = _tmp1*_tmp2
        _tmp4 = _tmp2*(-_tmp0 + 1)

        # Output terms
        _res = [0.] * 4
        _res[0] = _tmp0
        _res[1] = _tmp1
        _res[2] = _tmp3*vec[0] - _tmp4*vec[1]
        _res[3] = _tmp3*vec[1] + _tmp4*vec[0]
        return _res

    @staticmethod
    def to_tangent(a, epsilon):

        # Input arrays
        _a = a.data

        # Intermediate terms
        _tmp0 = numpy.arctan2(_a[1], _a[0] + epsilon*(numpy.sign(_a[0]) + 0.5))
        _tmp1 = 0.5*_tmp0
        _tmp2 = _a[2]*_tmp1
        _tmp3 = _a[1]/numpy.amax((epsilon,-_a[0] + 1))
        _tmp4 = _a[3]*_tmp1

        # Output terms
        _res = [0.] * 3
        _res[0] = _tmp2*_tmp3 + _tmp4
        _res[1] = -_tmp2 + _tmp3*_tmp4
        _res[2] = _tmp0
        return _res

    @staticmethod
    def retract(a, vec, epsilon):

        # Input arrays
        _a = a.data

        # Intermediate terms
        _tmp0 = numpy.cos(vec[2])
        _tmp1 = numpy.sin(vec[2])
        _tmp2 = 1/(epsilon + vec[2])
        _tmp3 = _tmp1*_tmp2
        _tmp4 = _tmp2*(-_tmp0 + 1)
        _tmp5 = _tmp3*vec[0] - _tmp4*vec[1]
        _tmp6 = _tmp3*vec[1] + _tmp4*vec[0]

        # Output terms
        _res = [0.] * 4
        _res[0] = _a[0]*_tmp0 - _a[1]*_tmp1
        _res[1] = _a[0]*_tmp1 + _a[1]*_tmp0
        _res[2] = _a[0]*_tmp5 - _a[1]*_tmp6 + _a[2]
        _res[3] = _a[0]*_tmp6 + _a[1]*_tmp5 + _a[3]
        return _res

    @staticmethod
    def local_coordinates(a, b, epsilon):

        # Input arrays
        _a = a.data
        _b = b.data

        # Intermediate terms
        _tmp0 = 1/(_a[0]**2 + _a[1]**2)
        _tmp1 = _a[0]*_tmp0
        _tmp2 = _a[1]*_tmp0
        _tmp3 = -_b[0]*_tmp2 + _b[1]*_tmp1
        _tmp4 = _b[0]*_tmp1 + _b[1]*_tmp2
        _tmp5 = numpy.arctan2(_tmp3, _tmp4 + epsilon*(numpy.sign(_tmp4) + 0.5))
        _tmp6 = 0.5*_tmp5
        _tmp7 = _tmp6*(-_a[2]*_tmp1 - _a[3]*_tmp2 + _b[2]*_tmp1 + _b[3]*_tmp2)
        _tmp8 = _tmp3/numpy.amax((epsilon,-_tmp4 + 1))
        _tmp9 = _tmp6*(_a[2]*_tmp2 - _a[3]*_tmp1 - _b[2]*_tmp2 + _b[3]*_tmp1)

        # Output terms
        _res = [0.] * 3
        _res[0] = _tmp7*_tmp8 + _tmp9
        _res[1] = -_tmp7 + _tmp8*_tmp9
        _res[2] = _tmp5
        return _res
