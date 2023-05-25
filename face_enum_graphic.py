from pathlib import Path

from face_enumeration import Interface_KP

from graphviz import Digraph


class FaceEnum():
    def __init__(self, path_to_file: Path | None) -> None:
        if path_to_file:
            vector_interpretation = self.read_from_file(path_to_file)
        else:
            vector_interpretation = []
        self.enumeration_interface = Interface_KP(vector_interpretation)
        self.enumeration_interface.ConvertToData()
        self.is_enumerated = False

    @staticmethod
    def read_from_file(path: Path) -> list[int]:
        result_data_vector = []
        with open(path, 'r') as fl:
            lines = fl.readlines()
            for line in lines:
                data_list= list(map(lambda x: int(x),line.split()))
                result_data_vector.append(data_list)
        result_data_vector[3:] = sorted(result_data_vector[3:])
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

        dim_store[tuple(self.enumeration_interface.V_set)] = self.enumeration_interface.dim_ + 1
        dim_store = sorted(dim_store.items(), key=lambda x: x[1])
        

        list_enterpretation = []

        for num, atom in enumerate(dim_store):
            atom_dim = atom[1]
            face = atom[0]
            for coatom, coatom_dim in dim_store[num + 1:]:
                if coatom_dim > atom_dim + 1:
                    break
                intersection = set(coatom).intersection(face)
                if tuple(intersection) == face:
                    list_enterpretation.append((face, coatom))
        
        str_list_enterpretation = [(' '.join(str(x) for x in face1), ' '.join(str(x) for x in face2))
                                   for face1, face2 in list_enterpretation]
        graph_interpret = Digraph('Hasse_diagram', filename='Hasse_diagram.gv', engine="dot")
        for fc1, fc2 in str_list_enterpretation:
            graph_interpret.edge(fc1, fc2)

        graph_interpret.view()


if __name__ == "__main__":
    test_face_enum = FaceEnum(Path.cwd() / 'Hasse_diagram' / 'examples' / 'pyr4.txt')
    test_face_enum.face_enumeration()
    test_face_enum.print_text_hasse_diagramm()
    test_face_enum.draw_hasse_diagram()