import pathlib

import numpy as np
from face_enumeration import Interface_KP
from graphviz import Digraph


class FaceEnumeration():
    """
    Special class for face enuumeration calculation.
    """

    def __init__(self, incidence_matrix, poly_parameters: tuple | None = None) -> None:
        """
        That constructor get incidence matrix in NumPy 2d array, list of incedence or path to file.

        Args:

            - incidence_matrix: Path, list or ndarray: list of list[int] with vertex-facet incidence.
            - poly_parameters(needs only for list or ndarray incidence_matrix): list with
            [facet_number, vertex_number, dimersation]
        """
        if isinstance(incidence_matrix, pathlib.PurePath) or isinstance(incidence_matrix, str):
            vector_interpretation = self._read_from_file(incidence_matrix)
        elif isinstance(incidence_matrix, list):
            if poly_parameters:
                vector_interpretation = self._read_list_incidence(
                    incidence_matrix, poly_parameters)
            else:
                raise ValueError('poly_parameters is wrong or empty. Please, enter parameters correctly')
        elif isinstance(incidence_matrix, np.ndarray):
            if poly_parameters:
                vector_interpretation = self._read_np_array_incidence(
                    incidence_matrix, poly_parameters)
            else:
                raise ValueError('poly_parameters is wrong or empty. Please, enter parameters correctly')
        else:
            raise TypeError('Wrong type. Please use list, Path or ndarray.')

        self.enumeration_interface = Interface_KP(vector_interpretation)
        self.enumeration_interface.ConvertToData()
        self.is_enumerated = False

    @staticmethod
    def _read_from_file(path: pathlib.PurePath | str) -> list[list[int]]:
        result_data_vector = []
        with open(path, 'r') as fl:
            lines = fl.readlines()
            for line in lines:
                data_list = list(map(lambda x: int(x), line.split()))
                result_data_vector.append(data_list)
        result_data_vector[3:] = sorted(result_data_vector[3:])
        return result_data_vector

    @staticmethod
    def _read_list_incidence(incidence_list, poly_parameters: tuple[int]) -> list[list[int]]:
        result_data_vector = [[x] for x in poly_parameters]
        # result_data_vector.append([x] for x in poly_parameters)
        result_data_vector.extend(incidence_list)

        return result_data_vector

    @staticmethod
    def _read_np_array_incidence(np_incidence: np.ndarray, poly_parameters: tuple[int]) -> list[list[int]]:
        result_data_vector = []
        result_data_vector.append([x] for x in poly_parameters)
        result_data_vector.extend(np_incidence.tolist())
        return result_data_vector

    def face_enumeration(self):
        """
        Run C++ implementation of face enumeration
        """
        if not self.is_enumerated:
            self.enumeration_interface.FindAllFace()
            self.is_enumerated = True

    def print_text_hasse_diagramm(self):
        """
        Print Hasse diagramm to console in text form
        """
        self.face_enumeration()
        self.enumeration_interface.Output()

    def draw_hasse_diagram(self):
        """
        Process atom-coatom structure and build graph
        """
        self.face_enumeration()
        dimer_store = self.enumeration_interface.dimersation_store
        dim_store = dict()
        for elem, val in dimer_store.items():
            dim_store[tuple(elem.vertices)] = val

        dim_store[tuple(self.enumeration_interface.V_set)
                  ] = self.enumeration_interface.dim_
        dim_store = sorted(dim_store.items(), key=lambda x: x[1])

        list_enterpretation = []

        for num, atom in enumerate(dim_store):
            atom_dim = atom[1]
            face = atom[0]
            for coatom, coatom_dim in dim_store[num + 1:]:
                if coatom_dim > atom_dim + 1:
                    break
                intersection = set(coatom).intersection(face)
                intersection = sorted(intersection)
                if tuple(intersection) == face:
                    list_enterpretation.append((face, coatom))

        str_list_enterpretation = [(' '.join(str(x) for x in face1), ' '.join(str(x) for x in face2))
                                   for face1, face2 in list_enterpretation]
        graph_interpret = Digraph('Hasse_diagram', format='pdf', engine="dot", graph_attr={"concentrate": "true"})
        for fc1, fc2 in str_list_enterpretation:
            graph_interpret.edge(fc1, fc2)

        return graph_interpret

    def view_graph(self, graph: Digraph):
        graph.view()



if __name__ == "__main__":
    test_face_enum = FaceEnumeration(
        pathlib.Path.cwd() / 'Hasse_diagram' / 'examples' / 'pyr4.txt')
    test_face_enum.print_text_hasse_diagramm()
    graph = test_face_enum.draw_hasse_diagram()
    graph.view()
